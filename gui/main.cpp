#include <QApplication>
#include <QAbstractItemView>
#include <QSizePolicy>
#include <QStringList>
#include <QCheckBox>
#include <QComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFont>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QGroupBox>
#include <QFrame>
#include <QTextStream>
#include <QRegularExpression>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <functional>
#include <limits>
#include <vector>
#include "prime_checker.h"
#include "probabilistic_primality.h"

struct Row {
    QString algorithm;
    QString result;
    QString checks;
    double ms = 0.0;
};

template<class F>
double medianMs(F fn) {
    std::vector<double> samples;
    for (int i = 0; i < 5; ++i) {
        const auto start = std::chrono::steady_clock::now();
        fn();
        const auto stop = std::chrono::steady_clock::now();
        samples.push_back(std::chrono::duration<double, std::milli>(stop-start).count());
    }
    std::sort(samples.begin(), samples.end());
    return samples[samples.size()/2];
}

class BarChart final : public QWidget {
public:
    explicit BarChart(QWidget *parent=nullptr): QWidget(parent) {
        setMinimumHeight(210);
        setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    }
    void setRows(const std::vector<Row>& rows) { rows_=rows; update(); }
protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.fillRect(rect(),QColor("#0b1930"));
        p.setPen(QColor("#9fb5d4"));
        p.drawText(14,22,"Bajarilish vaqti (ms) — kichik qiymat tezroq");
        if(rows_.empty()) {
            p.setPen(QColor("#8194ad"));
            p.drawText(rect(),Qt::AlignCenter,"Diagramma uchun avval algoritmlarni ishga tushiring");
            return;
        }
        const int left=165, right=width()-24, top=42, bottom=height()-25;
        const double maxValue=std::max(0.000001,(*std::max_element(rows_.begin(),rows_.end(),
            [](const Row&a,const Row&b){return a.ms<b.ms;})).ms);
        const int slot=std::max(25,(bottom-top)/static_cast<int>(rows_.size()));
        const QColor colors[]={QColor("#2693ff"),QColor("#20c997"),QColor("#f5a623"),QColor("#9b6bff"),QColor("#00c8df")};
        for(size_t i=0;i<rows_.size();++i) {
            const int y=top+static_cast<int>(i)*slot;
            p.setPen(QColor("#d8e5f7"));
            p.drawText(QRect(8,y,left-16,slot),Qt::AlignVCenter|Qt::AlignRight,rows_[i].algorithm);
            const int available=std::max(1,right-left);
            const int bar=static_cast<int>((rows_[i].ms/maxValue)*available);
            p.setPen(Qt::NoPen); p.setBrush(colors[i%5]);
            p.drawRoundedRect(QRect(left,y+slot/4,std::max(2,bar),std::max(8,slot/2)),5,5);
            p.setPen(QColor("#e7f0ff"));
            p.drawText(left+bar+7,y,90,slot,Qt::AlignVCenter,QString::number(rows_[i].ms,'f',6));
        }
    }
private:
    std::vector<Row> rows_;
};

int main(int argc,char *argv[]) {
    QApplication app(argc,argv);
    QWidget window;
    window.setWindowTitle("Matematik Asoslar | Tub sonlarni tekshirish");
    window.resize(1180,820);
    window.setStyleSheet(R"(
        QWidget { background:#081426; color:#e8f1ff; font-family:"Segoe UI"; font-size:14px; }
        QLabel#heading { color:#64b5ff; font-size:27px; font-weight:700; }
        QLabel#muted { color:#9db2ce; }
        QGroupBox { border:1px solid #24466e; border-radius:12px; margin-top:12px; padding:14px; font-weight:600; }
        QGroupBox::title { subcontrol-origin:margin; left:14px; padding:0 7px; color:#8fc8ff; }
        QLineEdit { background:#102542; border:1px solid #37618f; border-radius:7px; padding:11px; color:white; font-size:17px; }
        QCheckBox { spacing:9px; padding:5px; }
        QCheckBox::indicator { width:18px; height:18px; }
        QPushButton { background:#1268d8; border:0; border-radius:8px; padding:11px 18px; font-weight:700; }
        QPushButton:hover { background:#2585f5; }
        QPushButton#secondary { background:#1b3555; border:1px solid #41658e; }
        QTableWidget { background:#0d2039; alternate-background-color:#102744; gridline-color:#294666; border:1px solid #294666; border-radius:8px; }
        QHeaderView::section { background:#17365b; color:#e8f1ff; padding:9px; border:0; font-weight:700; }
        QTableWidget::item { padding:7px; }
    )");

    auto *root=new QVBoxLayout(&window);
    root->setContentsMargins(22,18,22,18); root->setSpacing(13);
    auto *heading=new QLabel("π  Tub sonlarni tekshirish laboratoriyasi");
    heading->setObjectName("heading"); root->addWidget(heading);
    auto *subtitle=new QLabel("Turli algoritmlarning aniqligi va ishlash tezligini taqqoslash");
    subtitle->setObjectName("muted"); root->addWidget(subtitle);

    auto *top=new QHBoxLayout;
    auto *inputBox=new QGroupBox("Kirish ma'lumotlari");
    auto *inputLayout=new QVBoxLayout(inputBox);
    auto *input=new QLineEdit;
    input->setPlaceholderText("Butun son kiriting: masalan 9973");
    inputLayout->addWidget(new QLabel("Son (0 ... 18446744073709551615):"));
    inputLayout->addWidget(input);
    auto *hint=new QLabel("Eslatma: sekin sinov-bo'linish usullari 10¹² dan katta sonlarda o'tkazib yuboriladi.");
    hint->setObjectName("muted"); hint->setWordWrap(true); inputLayout->addWidget(hint);
    top->addWidget(inputBox,2);

    auto *algorithmBox=new QGroupBox("Tekshiriladigan algoritmlar");
    auto *algorithmLayout=new QVBoxLayout(algorithmBox);
    auto *checks=new std::vector<QCheckBox*>();
    const QStringList names={"Oddiy sinov bo‘linish","6k ± 1 algoritmi","Fermat testi (10 raund)","Miller–Rabin (10 raund)","Deterministik Miller–Rabin"};
    for(const QString &name:names) { auto *cb=new QCheckBox(name); cb->setChecked(true); algorithmLayout->addWidget(cb); checks->push_back(cb); }
    top->addWidget(algorithmBox,2);
    root->addLayout(top);

    auto *actions=new QHBoxLayout;
    auto *run=new QPushButton("▶  Tekshirish");
    auto *clear=new QPushButton("Tozalash"); clear->setObjectName("secondary");
    auto *exportButton=new QPushButton("CSV eksport"); exportButton->setObjectName("secondary");
    actions->addWidget(run); actions->addWidget(clear); actions->addStretch(); actions->addWidget(exportButton);
    root->addLayout(actions);

    auto *resultsBox=new QGroupBox("Natijalar");
    auto *resultsLayout=new QVBoxLayout(resultsBox);
    auto *table=new QTableWidget(0,4);
    table->setHorizontalHeaderLabels({"Algoritm","Natija","Tekshiruv / raund","Vaqt (ms)"});
    table->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeToContents);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    resultsLayout->addWidget(table);
    root->addWidget(resultsBox,2);

    auto *chartBox=new QGroupBox("Algoritmlar tezligi taqqoslanishi");
    auto *chartLayout=new QVBoxLayout(chartBox);
    auto *chart=new BarChart; chartLayout->addWidget(chart);
    root->addWidget(chartBox,2);

    auto *status=new QLabel("Tayyor — son kiriting va algoritmlarni tanlang.");
    status->setObjectName("muted"); root->addWidget(status);

    std::vector<Row> latestRows;
    uint64_t latestNumber=0;
    QObject::connect(run,&QPushButton::clicked,[&](){
        const QString raw=input->text().trimmed();
        static const QRegularExpression digits("^[0-9]+$");
        if(!digits.match(raw).hasMatch()) {
            QMessageBox::warning(&window,"Kiritish xatosi","Faqat manfiy bo'lmagan butun son kiriting."); return;
        }
        bool ok=false; const qulonglong parsed=raw.toULongLong(&ok,10);
        if(!ok) { QMessageBox::warning(&window,"Kiritish xatosi","Son uint64 diapazonidan tashqarida."); return; }
        if(std::none_of(checks->begin(),checks->end(),[](QCheckBox*c){return c->isChecked();})) {
            QMessageBox::information(&window,"Tanlov","Kamida bitta algoritmni tanlang."); return;
        }
        const uint64_t n=static_cast<uint64_t>(parsed);
        latestNumber=n; latestRows.clear(); table->setRowCount(0);
        auto add=[&](const QString&name,const QString&result,const QString&count,double ms){
            latestRows.push_back({name,result,count,ms});
        };
        if((*checks)[0]->isChecked()) {
            if(n>1000000000000ULL) add(names[0],"O'tkazib yuborildi","Katta son",0);
            else { uint64_t d=0,c=0; bool prime=false; double ms=medianMs([&]{prime=isPrime(n,d,c);});
                add(names[0],prime?"Tub son":"Murakkab son",QString::number(c),ms); }
        }
        if((*checks)[1]->isChecked()) {
            if(n>1000000000000ULL) add(names[1],"O'tkazib yuborildi","Katta son",0);
            else { uint64_t d=0,c=0; bool prime=false; double ms=medianMs([&]{prime=isPrime6k1(n,d,c);});
                add(names[1],prime?"Tub son":"Murakkab son",QString::number(c),ms); }
        }
        if((*checks)[2]->isChecked()) {
            ProbablePrimeResult r; double ms=medianMs([&]{r=fermatTest(n,10,2026);});
            add(names[2],r.probablePrime?"Ehtimol tub":"Murakkab",QString::number(r.roundsRun),ms);
        }
        if((*checks)[3]->isChecked()) {
            ProbablePrimeResult r; double ms=medianMs([&]{r=millerRabinTest(n,10,2026);});
            add(names[3],r.probablePrime?"Ehtimol tub":"Murakkab",QString::number(r.roundsRun),ms);
        }
        if((*checks)[4]->isChecked()) {
            bool prime=false; double ms=medianMs([&]{prime=isPrimeDeterministic64(n);});
            add(names[4],prime?"Tub son":"Murakkab son","7 ta asos",ms);
        }
        for(const Row&r:latestRows) {
            const int row=table->rowCount(); table->insertRow(row);
            table->setItem(row,0,new QTableWidgetItem(r.algorithm));
            table->setItem(row,1,new QTableWidgetItem(r.result));
            table->setItem(row,2,new QTableWidgetItem(r.checks));
            table->setItem(row,3,new QTableWidgetItem(r.ms>0?QString::number(r.ms,'f',6):"—"));
        }
        std::vector<Row> chartRows;
        for(const Row&r:latestRows) if(r.ms>0) chartRows.push_back(r);
        chart->setRows(chartRows);
        status->setText(QString("Tayyor — %1 soni %2 ta algoritm bilan tekshirildi. Vaqt: 5 takrorning medianasi.")
                        .arg(QString::number(n)).arg(latestRows.size()));
    });
    QObject::connect(clear,&QPushButton::clicked,[&](){
        input->clear(); table->setRowCount(0); latestRows.clear(); chart->setRows({});
        status->setText("Maydonlar tozalandi."); input->setFocus();
    });
    QObject::connect(exportButton,&QPushButton::clicked,[&](){
        if(latestRows.empty()) { QMessageBox::information(&window,"Hisobot","Avval tekshiruvni bajaring."); return; }
        const QString path=QFileDialog::getSaveFileName(&window,"CSV hisobotni saqlash","tub_son_natijalari.csv","CSV fayllar (*.csv)");
        if(path.isEmpty()) return;
        QFile file(path);
        if(!file.open(QIODevice::WriteOnly|QIODevice::Text)) { QMessageBox::critical(&window,"Xato","Faylni saqlab bo'lmadi."); return; }
        QTextStream out(&file);
        out<<"son,algoritm,natija,tekshiruv_yoki_raund,vaqt_ms\n";
        for(const Row&r:latestRows) {
            QString name=r.algorithm; name.replace('"',"''");
            QString result=r.result; result.replace('"',"''");
            out<<QString::number(latestNumber)<<",\""<<name<<"\",\""<<result<<"\","<<r.checks<<",";
            if(r.ms>0) out<<QString::number(r.ms,'f',9); else out<<"";
            out<<"\n";
        }
        file.close();
        QMessageBox::information(&window,"Tayyor","CSV hisoboti muvaffaqiyatli saqlandi.");
    });

    window.show();
    return app.exec();
}
