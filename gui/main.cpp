#include <QApplication>
#include <QAbstractItemView>
#include <QCheckBox>
#include <QComboBox>
#include <QFile>
#include <QFileDialog>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QStackedWidget>
#include <QTableWidget>
#include <QTextStream>
#include <QVBoxLayout>
#include <QWidget>
#include <QGroupBox>
#include <QRegularExpression>
#include <QSettings>
#include <QFrame>
#include <QScrollArea>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <vector>

#include "prime_checker.h"
#include "probabilistic_primality.h"

struct ResultRow {
    QString algorithm;
    QString result;
    QString detail;
    double milliseconds = 0.0;
};

template <typename Function>
static double medianMilliseconds(Function function) {
    std::vector<double> samples;
    for (int i = 0; i < 5; ++i) {
        const auto start = std::chrono::steady_clock::now();
        function();
        const auto finish = std::chrono::steady_clock::now();
        samples.push_back(std::chrono::duration<double, std::milli>(finish - start).count());
    }
    std::sort(samples.begin(), samples.end());
    return samples[samples.size() / 2];
}

static QString csvField(QString value) {
    value.replace("\"", "\"\"");
    return "\"" + value + "\"";
}

class BarChart final : public QWidget {
public:
    explicit BarChart(QWidget *parent = nullptr) : QWidget(parent) {
        setMinimumHeight(280);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

    void setRows(const std::vector<ResultRow>& rows) {
        rows_ = rows;
        update();
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.fillRect(rect(), QColor("#101f35"));
        painter.setPen(QColor("#a9bdd7"));
        painter.drawText(16, 25, "Median bajarilish vaqti (ms) — kichik qiymat tezroq");

        if (rows_.empty()) {
            painter.drawText(rect(), Qt::AlignCenter,
                             "Hozircha diagramma uchun natija yo'q. Avval tekshiruv bajaring.");
            return;
        }

        double maximum = 0.0;
        for (const auto& row : rows_) maximum = std::max(maximum, row.milliseconds);
        maximum = std::max(maximum, 0.000001);

        const int top = 52;
        const int left = 220;
        const int right = width() - 45;
        const int availableWidth = std::max(1, right - left);
        const int slot = std::max(38, (height() - 75) / static_cast<int>(rows_.size()));
        const QColor colors[] = {
            QColor("#369cff"), QColor("#20c997"), QColor("#f0ad4e"),
            QColor("#a78bfa"), QColor("#22c7d8")
        };

        for (size_t i = 0; i < rows_.size(); ++i) {
            const int y = top + static_cast<int>(i) * slot;
            painter.setPen(QColor("#e5eefb"));
            painter.drawText(QRect(8, y, left - 18, slot),
                             Qt::AlignRight | Qt::AlignVCenter,
                             rows_[i].algorithm);
            const int barWidth = static_cast<int>(
                availableWidth * rows_[i].milliseconds / maximum);
            painter.setPen(Qt::NoPen);
            painter.setBrush(colors[i % 5]);
            painter.drawRoundedRect(left, y + slot / 4,
                                    std::max(2, barWidth), slot / 2, 6, 6);
            painter.setPen(QColor("#e5eefb"));
            painter.drawText(left + barWidth + 8, y, 110, slot,
                             Qt::AlignVCenter,
                             QString::number(rows_[i].milliseconds, 'g', 6));
        }
    }

private:
    std::vector<ResultRow> rows_;
};

class MainWindow final : public QMainWindow {
public:
    MainWindow() {
        setWindowTitle("Tub Sonlar Laboratoriyasi | Matematik asoslar");
        resize(1280, 820);
        setMinimumSize(1000, 680);

        auto *central = new QWidget(this);
        setCentralWidget(central);
        auto *outer = new QHBoxLayout(central);
        outer->setContentsMargins(0, 0, 0, 0);
        outer->setSpacing(0);

        auto *sidebar = new QFrame;
        sidebar->setObjectName("sidebar");
        sidebar->setFixedWidth(245);
        auto *sideLayout = new QVBoxLayout(sidebar);
        sideLayout->setContentsMargins(18, 24, 18, 18);
        sideLayout->setSpacing(9);

        auto *brand = new QLabel("π  PRIME LAB");
        brand->setObjectName("brand");
        sideLayout->addWidget(brand);
        auto *tagline = new QLabel("MATEMATIK ASOSLAR");
        tagline->setObjectName("muted");
        sideLayout->addWidget(tagline);
        sideLayout->addSpacing(22);

        stack_ = new QStackedWidget;
        const QStringList navigation = {
            "Bosh sahifa",
            "Sonni tekshirish",
            "Statistika va diagrammalar",
            "Tekshiruvlar tarixi",
            "Hisobotlar",
            "Algoritmlar haqida",
            "Sozlamalar",
            "Dastur haqida"
        };

        for (int i = 0; i < navigation.size(); ++i) {
            auto *button = new QPushButton(navigation[i]);
            button->setObjectName("navButton");
            button->setCheckable(true);
            sideLayout->addWidget(button);
            navigationButtons_.push_back(button);
            connect(button, &QPushButton::clicked, this,
                    [this, i]() { showPage(i); });
        }

        sideLayout->addStretch();
        auto *version = new QLabel("C++17  •  Qt 6\nWindows desktop");
        version->setObjectName("muted");
        sideLayout->addWidget(version);

        outer->addWidget(sidebar);
        outer->addWidget(stack_, 1);

        buildHomePage();
        buildCheckerPage();
        buildStatisticsPage();
        buildHistoryPage();
        buildReportsPage();
        buildAlgorithmsPage();
        buildSettingsPage();
        buildAboutPage();

        showPage(0);
        applyTheme("To'q");
    }

private:
    QStackedWidget *stack_ = nullptr;
    std::vector<QPushButton*> navigationButtons_;
    QLineEdit *numberInput_ = nullptr;
    std::vector<QCheckBox*> algorithmChecks_;
    QTableWidget *resultsTable_ = nullptr;
    BarChart *chart_ = nullptr;
    QListWidget *historyList_ = nullptr;
    QLabel *checkerStatus_ = nullptr;
    QLabel *homeSummary_ = nullptr;
    QLabel *reportSummary_ = nullptr;
    QComboBox *themeChoice_ = nullptr;
    std::vector<ResultRow> latestResults_;
    std::vector<QString> historyEntries_;
    uint64_t latestNumber_ = 0;
    uint64_t totalChecks_ = 0;

    QWidget *createPage(const QString& title, const QString& subtitle) {
        auto *widget = new QWidget;
        auto *layout = new QVBoxLayout(widget);
        layout->setContentsMargins(30, 25, 30, 25);
        layout->setSpacing(15);

        auto *heading = new QLabel(title);
        heading->setObjectName("pageTitle");
        layout->addWidget(heading);

        auto *description = new QLabel(subtitle);
        description->setObjectName("muted");
        description->setWordWrap(true);
        layout->addWidget(description);

        stack_->addWidget(widget);
        return widget;
    }

    static QLabel *createInfoCard(const QString& title, const QString& description) {
        auto *label = new QLabel(
            "<b>" + title.toHtmlEscaped() + "</b><br><span style='color:#9db2ce'>" +
            description.toHtmlEscaped() + "</span>");
        label->setObjectName("infoCard");
        label->setWordWrap(true);
        label->setMinimumHeight(90);
        return label;
    }

    void showPage(int index) {
        stack_->setCurrentIndex(index);
        for (size_t i = 0; i < navigationButtons_.size(); ++i)
            navigationButtons_[i]->setChecked(static_cast<int>(i) == index);
    }

    void buildHomePage() {
        auto *widget = createPage(
            "Bosh sahifa",
            "Tub sonlarni tekshirish algoritmlarini o'rganish va ularning samaradorligini taqqoslash.");
        auto *layout = qobject_cast<QVBoxLayout*>(widget->layout());

        auto *welcome = new QLabel("Matematik algoritmlar laboratoriyasiga xush kelibsiz");
        welcome->setObjectName("welcome");
        welcome->setWordWrap(true);
        layout->addWidget(welcome);

        auto *cards = new QHBoxLayout;
        cards->addWidget(createInfoCard(
            "01  Sonni tekshirish",
            "Son kiriting, algoritmlarni tanlang va natijalarni alohida jadvalda ko'ring."));
        cards->addWidget(createInfoCard(
            "02  Statistika",
            "Algoritmlarning bajarilish vaqtini diagrammada solishtiring."));
        cards->addWidget(createInfoCard(
            "03  Tekshiruvlar tarixi",
            "Joriy dastur sessiyasidagi avvalgi tekshiruvlarni ko'ring."));
        layout->addLayout(cards);

        homeSummary_ = new QLabel("Hali tekshiruv bajarilmadi.");
        homeSummary_->setObjectName("infoCard");
        homeSummary_->setWordWrap(true);
        layout->addWidget(homeSummary_);

        auto *openChecker = new QPushButton("Sonni tekshirish sahifasini ochish  →");
        openChecker->setObjectName("primary");
        connect(openChecker, &QPushButton::clicked, this,
                [this]() { showPage(1); });
        layout->addWidget(openChecker);
        layout->addStretch();
    }

    void buildCheckerPage() {
        auto *widget = createPage(
            "Sonni tekshirish",
            "Sonni kiriting va kerakli algoritmlarni belgilang. Ehtimollik testlari aniq natija emas, 'ehtimol tub' xulosasini beradi.");
        auto *layout = qobject_cast<QVBoxLayout*>(widget->layout());

        auto *top = new QHBoxLayout;
        auto *inputBox = new QGroupBox("Kirish ma'lumoti");
        auto *inputLayout = new QVBoxLayout(inputBox);
        inputLayout->addWidget(new QLabel("0 dan 18446744073709551615 gacha bo'lgan butun son:"));
        numberInput_ = new QLineEdit;
        numberInput_->setPlaceholderText("Masalan: 9973");
        inputLayout->addWidget(numberInput_);
        inputLayout->addWidget(createInfoCard(
            "Eslatma",
            "Oddiy va 6k ± 1 usullari 10^12 dan katta sonlarda vaqtni tejash uchun o'tkazib yuboriladi."));
        top->addWidget(inputBox, 2);

        auto *algorithmBox = new QGroupBox("Tekshiriladigan algoritmlar");
        auto *algorithmLayout = new QVBoxLayout(algorithmBox);
        const QStringList names = {
            "Oddiy sinov bo'linish",
            "6k ± 1 algoritmi",
            "Fermat testi (10 raund)",
            "Miller–Rabin (10 raund)",
            "Deterministik Miller–Rabin"
        };
        for (const QString& name : names) {
            auto *check = new QCheckBox(name);
            check->setChecked(true);
            algorithmLayout->addWidget(check);
            algorithmChecks_.push_back(check);
        }
        top->addWidget(algorithmBox, 2);
        layout->addLayout(top);

        auto *actions = new QHBoxLayout;
        auto *runButton = new QPushButton("▶  Tekshirish");
        runButton->setObjectName("primary");
        auto *clearButton = new QPushButton("Tozalash");
        clearButton->setObjectName("secondary");
        actions->addWidget(runButton);
        actions->addWidget(clearButton);
        actions->addStretch();
        layout->addLayout(actions);

        auto *tableBox = new QGroupBox("Natijalar jadvali");
        auto *tableLayout = new QVBoxLayout(tableBox);
        resultsTable_ = new QTableWidget(0, 4);
        resultsTable_->setHorizontalHeaderLabels(
            {"Algoritm", "Natija", "Tekshiruv / raund", "Vaqt (ms)"});
        resultsTable_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        for (int column = 1; column < 4; ++column)
            resultsTable_->horizontalHeader()->setSectionResizeMode(
                column, QHeaderView::ResizeToContents);
        resultsTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
        resultsTable_->setAlternatingRowColors(true);
        tableLayout->addWidget(resultsTable_);
        layout->addWidget(tableBox, 1);

        checkerStatus_ = new QLabel("Tayyor.");
        checkerStatus_->setObjectName("muted");
        layout->addWidget(checkerStatus_);

        connect(runButton, &QPushButton::clicked, this, [this]() { runCheck(); });
        connect(clearButton, &QPushButton::clicked, this, [this]() {
            numberInput_->clear();
            resultsTable_->setRowCount(0);
            latestResults_.clear();
            if (chart_) chart_->setRows({});
            checkerStatus_->setText("Maydonlar tozalandi.");
            numberInput_->setFocus();
        });
    }

    void buildStatisticsPage() {
        auto *widget = createPage(
            "Statistika va diagrammalar",
            "Oxirgi tekshiruvdagi algoritmlar bajarilish vaqtining taqqoslanishi.");
        auto *layout = qobject_cast<QVBoxLayout*>(widget->layout());
        chart_ = new BarChart;
        layout->addWidget(chart_, 1);
        layout->addWidget(createInfoCard(
            "O'lchash usuli",
            "Har bir tanlangan algoritm besh marta ishga tushiriladi va median vaqt ko'rsatiladi. Juda qisqa o'lchovlar kompyuter va tizim yuklamasiga bog'liq."));
        auto *back = new QPushButton("Yangi tekshiruv bajarish");
        back->setObjectName("primary");
        connect(back, &QPushButton::clicked, this, [this]() { showPage(1); });
        layout->addWidget(back);
    }

    void buildHistoryPage() {
        auto *widget = createPage(
            "Tekshiruvlar tarixi",
            "Tarix ushbu dastur oynasi ochiq turgan sessiya davomida saqlanadi.");
        auto *layout = qobject_cast<QVBoxLayout*>(widget->layout());
        historyList_ = new QListWidget;
        historyList_->setAlternatingRowColors(true);
        layout->addWidget(historyList_, 1);

        auto *clear = new QPushButton("Tarixni tozalash");
        clear->setObjectName("secondary");
        connect(clear, &QPushButton::clicked, this, [this]() {
            historyEntries_.clear();
            historyList_->clear();
            totalChecks_ = 0;
            updateSummary();
        });
        layout->addWidget(clear);
    }

    void buildReportsPage() {
        auto *widget = createPage(
            "Hisobotlar",
            "Oxirgi tekshiruv natijalarini CSV formatida saqlang. Faylni Excel yoki boshqa jadval dasturida ochish mumkin.");
        auto *layout = qobject_cast<QVBoxLayout*>(widget->layout());
        reportSummary_ = new QLabel("Hozircha eksport qilish uchun natija yo'q.");
        reportSummary_->setObjectName("infoCard");
        reportSummary_->setWordWrap(true);
        layout->addWidget(reportSummary_);

        auto *exportButton = new QPushButton("↓  Oxirgi natijalarni CSV formatida saqlash");
        exportButton->setObjectName("primary");
        connect(exportButton, &QPushButton::clicked, this,
                [this]() { exportLatestCsv(); });
        layout->addWidget(exportButton);
        layout->addWidget(createInfoCard(
            "CSV ustunlari",
            "Son, algoritm nomi, natija, tekshiruv yoki raundlar soni va bajarilish vaqti."));
        layout->addStretch();
    }

    void buildAlgorithmsPage() {
        auto *widget = createPage(
            "Algoritmlar haqida",
            "Loyihada besh xil yondashuv solishtiriladi. Vaqt murakkabligi soddalashtirilgan nazariy bahodir.");
        auto *layout = qobject_cast<QVBoxLayout*>(widget->layout());
        QScrollArea *scroll = new QScrollArea;
        scroll->setWidgetResizable(true);
        auto *content = new QWidget;
        auto *items = new QVBoxLayout(content);
        items->addWidget(createInfoCard(
            "1. Oddiy sinov bo'linish",
            "2 dan √n gacha bo'luvchilarni tekshiradi. Vaqt murakkabligi: O(√n). Deterministik usul."));
        items->addWidget(createInfoCard(
            "2. 6k ± 1",
            "2 va 3 ga bo'linishni alohida tekshiradi, keyin 6k−1 va 6k+1 nomzodlarini ko'radi. Vaqt murakkabligi: O(√n). Deterministik usul."));
        items->addWidget(createInfoCard(
            "3. Fermat testi",
            "Fermat kichik teoremasiga asoslangan ehtimollik testi. Ayrim murakkab sonlarni, jumladan Karmaykl sonlarini, tub deb qabul qilishi mumkin."));
        items->addWidget(createInfoCard(
            "4. Miller–Rabin",
            "Kuchli ehtimollik testi. Ko'proq raund odatda xato ehtimolini kamaytiradi, ammo bu variant natijasi baribir 'ehtimol tub' hisoblanadi."));
        items->addWidget(createInfoCard(
            "5. Deterministik Miller–Rabin",
            "Kodda belgilangan yetarli asoslar va 64-bit modul arifmetikasi bilan uint64 diapazonida deterministik tekshiruv uchun ishlatiladi."));
        items->addStretch();
        scroll->setWidget(content);
        layout->addWidget(scroll, 1);
    }

    void buildSettingsPage() {
        auto *widget = createPage(
            "Sozlamalar",
            "Dastur ko'rinishini o'zgartiring. Tanlangan mavzu joriy sessiya uchun amal qiladi.");
        auto *layout = qobject_cast<QVBoxLayout*>(widget->layout());
        auto *box = new QGroupBox("Ko'rinish");
        auto *boxLayout = new QVBoxLayout(box);
        themeChoice_ = new QComboBox;
        themeChoice_->addItems({"To'q", "Yorug'"});
        boxLayout->addWidget(new QLabel("Rang mavzusi:"));
        boxLayout->addWidget(themeChoice_);
        layout->addWidget(box);
        connect(themeChoice_, &QComboBox::currentTextChanged,
                this, [this](const QString& theme) { applyTheme(theme); });
        layout->addWidget(createInfoCard(
            "Aniqlik haqida",
            "Fermat va tasodifiy asosli Miller–Rabin natijalari 'ehtimol tub' deb ko'rsatiladi. Aniq xulosa uchun deterministik algoritmdan foydalaning."));
        layout->addStretch();
    }

    void buildAboutPage() {
        auto *widget = createPage(
            "Dastur haqida",
            "Matematik asoslar faniga mo'ljallangan o'quv loyihasi.");
        auto *layout = qobject_cast<QVBoxLayout*>(widget->layout());
        auto *title = new QLabel("TUB SONLAR LABORATORIYASI");
        title->setObjectName("welcome");
        layout->addWidget(title);
        layout->addWidget(createInfoCard(
            "Texnologiyalar",
            "C++17, Qt 6 Widgets va CMake. Windows uchun 64-bit desktop dastur sifatida yig'iladi."));
        layout->addWidget(createInfoCard(
            "Loyiha maqsadi",
            "Tub sonlarni aniqlashning deterministik va ehtimollik algoritmlarini o'rganish, ularning ishlash vaqtini solishtirish."));
        layout->addWidget(createInfoCard(
            "Mualliflik va foydalanish",
            "O'quv loyihasi. O'lchov natijalari apparat va operatsion tizimga qarab farq qilishi mumkin."));
        layout->addStretch();
    }

    void runCheck() {
        const QString raw = numberInput_->text().trimmed();
        static const QRegularExpression digits("^[0-9]+$");
        if (!digits.match(raw).hasMatch()) {
            QMessageBox::warning(this, "Kiritish xatosi",
                                 "Faqat manfiy bo'lmagan butun son kiriting.");
            return;
        }

        bool ok = false;
        const qulonglong parsed = raw.toULongLong(&ok, 10);
        if (!ok) {
            QMessageBox::warning(this, "Kiritish xatosi",
                                 "Son uint64 diapazonidan tashqarida.");
            return;
        }
        if (std::none_of(algorithmChecks_.begin(), algorithmChecks_.end(),
                         [](QCheckBox *check) { return check->isChecked(); })) {
            QMessageBox::information(this, "Algoritm tanlanmagan",
                                     "Kamida bitta algoritmni belgilang.");
            return;
        }

        const uint64_t n = static_cast<uint64_t>(parsed);
        latestNumber_ = n;
        latestResults_.clear();
        resultsTable_->setRowCount(0);

        const QStringList names = {
            "Oddiy sinov bo'linish",
            "6k ± 1 algoritmi",
            "Fermat testi (10 raund)",
            "Miller–Rabin (10 raund)",
            "Deterministik Miller–Rabin"
        };

        auto addResult = [this](const QString& algorithm, const QString& result,
                                const QString& detail, double milliseconds) {
            latestResults_.push_back({algorithm, result, detail, milliseconds});
        };

        if (algorithmChecks_[0]->isChecked()) {
            if (n > 1000000000000ULL) {
                addResult(names[0], "O'tkazib yuborildi", "Katta son", 0.0);
            } else {
                uint64_t divisor = 0, checks = 0;
                bool prime = false;
                const double ms = medianMilliseconds([&]() {
                    prime = isPrime(n, divisor, checks);
                });
                addResult(names[0], prime ? "Tub son" : "Murakkab son",
                          QString::number(checks), ms);
            }
        }

        if (algorithmChecks_[1]->isChecked()) {
            if (n > 1000000000000ULL) {
                addResult(names[1], "O'tkazib yuborildi", "Katta son", 0.0);
            } else {
                uint64_t divisor = 0, checks = 0;
                bool prime = false;
                const double ms = medianMilliseconds([&]() {
                    prime = isPrime6k1(n, divisor, checks);
                });
                addResult(names[1], prime ? "Tub son" : "Murakkab son",
                          QString::number(checks), ms);
            }
        }

        if (algorithmChecks_[2]->isChecked()) {
            ProbablePrimeResult result;
            const double ms = medianMilliseconds([&]() {
                result = fermatTest(n, 10, 2026);
            });
            addResult(names[2], result.probablePrime ? "Ehtimol tub" : "Murakkab",
                      QString::number(result.roundsRun), ms);
        }

        if (algorithmChecks_[3]->isChecked()) {
            ProbablePrimeResult result;
            const double ms = medianMilliseconds([&]() {
                result = millerRabinTest(n, 10, 2026);
            });
            addResult(names[3], result.probablePrime ? "Ehtimol tub" : "Murakkab",
                      QString::number(result.roundsRun), ms);
        }

        if (algorithmChecks_[4]->isChecked()) {
            bool prime = false;
            const double ms = medianMilliseconds([&]() {
                prime = isPrimeDeterministic64(n);
            });
            addResult(names[4], prime ? "Tub son" : "Murakkab son",
                      "7 ta asos", ms);
        }

        for (const auto& result : latestResults_) {
            const int row = resultsTable_->rowCount();
            resultsTable_->insertRow(row);
            resultsTable_->setItem(row, 0, new QTableWidgetItem(result.algorithm));
            resultsTable_->setItem(row, 1, new QTableWidgetItem(result.result));
            resultsTable_->setItem(row, 2, new QTableWidgetItem(result.detail));
            resultsTable_->setItem(
                row, 3, new QTableWidgetItem(
                    result.milliseconds > 0.0
                        ? QString::number(result.milliseconds, 'f', 6) : "—"));
        }

        std::vector<ResultRow> chartRows;
        for (const auto& result : latestResults_)
            if (result.milliseconds > 0.0) chartRows.push_back(result);
        if (chart_) chart_->setRows(chartRows);

        ++totalChecks_;
        const QString historyText = QString("%1  |  %2 ta algoritm")
            .arg(QString::number(n)).arg(latestResults_.size());
        historyEntries_.push_back(historyText);
        if (historyList_) historyList_->addItem(historyText);
        checkerStatus_->setText(
            QString("Tekshiruv yakunlandi. %1 ta algoritm bajarildi; vaqt — 5 takror medianasi.")
                .arg(latestResults_.size()));

        updateSummary();
    }

    void updateSummary() {
        const QString text = QString("Jami bajarilgan tekshiruvlar: %1\nOxirgi tekshirilgan son: %2")
            .arg(QString::number(totalChecks_))
            .arg(latestResults_.empty() ? QString("—") : QString::number(latestNumber_));
        if (homeSummary_) homeSummary_->setText(text);
        if (reportSummary_) {
            reportSummary_->setText(
                latestResults_.empty()
                    ? "Hozircha eksport qilish uchun natija yo'q."
                    : QString("Oxirgi son: %1\nNatijalar qatori: %2\nCSV eksportga tayyor.")
                        .arg(QString::number(latestNumber_))
                        .arg(latestResults_.size()));
        }
    }

    void exportLatestCsv() {
        if (latestResults_.empty()) {
            QMessageBox::information(this, "Hisobot",
                                     "Avval 'Sonni tekshirish' sahifasida tekshiruv bajaring.");
            return;
        }

        const QString path = QFileDialog::getSaveFileName(
            this, "CSV hisobotni saqlash", "tub_son_natijalari.csv",
            "CSV fayllar (*.csv)");
        if (path.isEmpty()) return;

        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Xato", "Faylni yozish uchun ochib bo'lmadi.");
            return;
        }

        QTextStream output(&file);
        output << "son,algoritm,natija,tekshiruv_yoki_raund,vaqt_ms\n";
        for (const auto& result : latestResults_) {
            output << QString::number(latestNumber_) << ','
                   << csvField(result.algorithm) << ','
                   << csvField(result.result) << ','
                   << csvField(result.detail) << ',';
            if (result.milliseconds > 0.0)
                output << QString::number(result.milliseconds, 'f', 9);
            output << '\n';
        }
        file.close();
        QMessageBox::information(this, "Tayyor",
                                 "CSV hisoboti muvaffaqiyatli saqlandi.");
    }

    void applyTheme(const QString& theme) {
        const bool light = theme.startsWith("Yorug");
        if (light) {
            qApp->setStyleSheet(R"(
                QWidget { background:#f1f5fa; color:#17263b; font-family:"Segoe UI"; font-size:14px; }
                QFrame#sidebar { background:#ffffff; border-right:1px solid #d5dfeb; }
                QLabel#brand { color:#1769aa; font-size:23px; font-weight:800; }
                QLabel#pageTitle { color:#173b63; font-size:27px; font-weight:700; }
                QLabel#welcome { color:#245d92; font-size:22px; font-weight:700; padding:18px; }
                QLabel#muted { color:#60748d; }
                QLabel#infoCard { background:#ffffff; border:1px solid #d6e1ee; border-radius:10px; padding:15px; }
                QGroupBox { background:#ffffff; border:1px solid #d1deec; border-radius:10px; margin-top:12px; padding:12px; font-weight:600; }
                QGroupBox::title { subcontrol-origin:margin; left:12px; padding:0 6px; color:#28669d; }
                QLineEdit, QComboBox { background:#ffffff; border:1px solid #bacde0; border-radius:7px; padding:10px; color:#17263b; }
                QPushButton { background:#e5edf6; border:1px solid #c4d3e3; border-radius:7px; padding:10px 12px; color:#173b63; text-align:left; }
                QPushButton:hover { background:#d7e8f8; }
                QPushButton#navButton { background:transparent; border:0; padding:11px; }
                QPushButton#navButton:checked { background:#dcecff; color:#125b9d; font-weight:700; }
                QPushButton#primary { background:#176fc1; color:white; border:0; font-weight:700; }
                QPushButton#secondary { background:#e5edf6; }
                QTableWidget, QListWidget { background:#ffffff; alternate-background-color:#f0f5fa; gridline-color:#d5e0eb; border:1px solid #d1deec; border-radius:8px; }
                QHeaderView::section { background:#e3edf7; color:#173b63; padding:9px; border:0; font-weight:700; }
            )");
        } else {
            qApp->setStyleSheet(R"(
                QWidget { background:#081426; color:#e8f1ff; font-family:"Segoe UI"; font-size:14px; }
                QFrame#sidebar { background:#0b1930; border-right:1px solid #203b5c; }
                QLabel#brand { color:#64b5ff; font-size:23px; font-weight:800; }
                QLabel#pageTitle { color:#64b5ff; font-size:27px; font-weight:700; }
                QLabel#welcome { color:#dcecff; font-size:22px; font-weight:700; padding:18px; }
                QLabel#muted { color:#9db2ce; }
                QLabel#infoCard { background:#10233e; border:1px solid #24466e; border-radius:10px; padding:15px; }
                QGroupBox { background:#0d1e35; border:1px solid #24466e; border-radius:10px; margin-top:12px; padding:12px; font-weight:600; }
                QGroupBox::title { subcontrol-origin:margin; left:12px; padding:0 6px; color:#8fc8ff; }
                QLineEdit, QComboBox { background:#102542; border:1px solid #37618f; border-radius:7px; padding:10px; color:white; }
                QPushButton { background:#1b3555; border:1px solid #345779; border-radius:7px; padding:10px 12px; color:#e8f1ff; text-align:left; }
                QPushButton:hover { background:#25486f; }
                QPushButton#navButton { background:transparent; border:0; padding:11px; }
                QPushButton#navButton:checked { background:#203d60; color:#ffffff; font-weight:700; }
                QPushButton#primary { background:#1268d8; color:white; border:0; font-weight:700; }
                QPushButton#primary:hover { background:#2585f5; }
                QPushButton#secondary { background:#1b3555; }
                QTableWidget, QListWidget { background:#0d2039; alternate-background-color:#102744; gridline-color:#294666; border:1px solid #294666; border-radius:8px; }
                QHeaderView::section { background:#17365b; color:#e8f1ff; padding:9px; border:0; font-weight:700; }
                QTableWidget::item { padding:7px; }
            )");
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    QApplication::setOrganizationName("PrimeLab");
    QApplication::setApplicationName("Tub Sonlar Laboratoriyasi");
    MainWindow window;
    window.show();
    return application.exec();
}
