#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <chrono>
#include "prime_checker.h"
#include "probabilistic_primality.h"

template<class F> double elapsedMs(F fn) {
    auto start = std::chrono::steady_clock::now();
    fn();
    return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now()-start).count();
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget window;
    window.setWindowTitle("Tub sonlar laboratoriyasi");
    window.resize(760, 560);
    auto *layout = new QVBoxLayout(&window);
    auto *title = new QLabel("TUB SONLARNI TAHLIL QILISH");
    QFont font; font.setPointSize(18); font.setBold(true); title->setFont(font);
    layout->addWidget(title);
    layout->addWidget(new QLabel("0 dan UINT64_MAX gacha butun son kiriting:"));
    auto *input = new QLineEdit;
    input->setPlaceholderText("Masalan: 1000003");
    layout->addWidget(input);
    auto *run = new QPushButton("Barcha algoritmlar bilan tekshirish");
    layout->addWidget(run);
    auto *output = new QPlainTextEdit;
    output->setReadOnly(true);
    layout->addWidget(output);
    auto *note = new QLabel("Fermat va tasodifiy Miller-Rabin ehtimollik testlari. Deterministik Miller-Rabin uint64 diapazonida aniq natija beradi.");
    note->setWordWrap(true);
    layout->addWidget(note);

    QObject::connect(run, &QPushButton::clicked, [&]() {
        bool ok = false;
        const auto parsed = input->text().trimmed().toULongLong(&ok, 10);
        if (!ok || input->text().trimmed().isEmpty()) {
            QMessageBox::warning(&window, "Kiritish xatosi", "0 dan UINT64_MAX gacha butun son kiriting.");
            return;
        }
        const uint64_t n = static_cast<uint64_t>(parsed);
        uint64_t d1=0,c1=0,d2=0,c2=0;
        bool p1=false,p2=false;
        const double t1=elapsedMs([&]{p1=isPrime(n,d1,c1);});
        const double t2=elapsedMs([&]{p2=isPrime6k1(n,d2,c2);});
        ProbablePrimeResult f,m;
        const double tf=elapsedMs([&]{f=fermatTest(n,10,2026);});
        const double tm=elapsedMs([&]{m=millerRabinTest(n,10,2026);});
        bool exact=false;
        const double td=elapsedMs([&]{exact=isPrimeDeterministic64(n);});
        QString text;
        text += QString("Kiritilgan son: %1\n\n").arg(QString::number(n));
        text += QString("1. Oddiy bo'linish: %1 | tekshiruv: %2 | %3 ms\n").arg(p1?"TUB":"TUB EMAS").arg(c1).arg(t1,0,'f',6);
        if (d1) text += QString("   Bo'luvchi: %1\n").arg(d1);
        text += QString("2. 6k +/- 1: %1 | tekshiruv: %2 | %3 ms\n").arg(p2?"TUB":"TUB EMAS").arg(c2).arg(t2,0,'f',6);
        if (d2) text += QString("   Bo'luvchi: %1\n").arg(d2);
        text += QString("3. Fermat: %1 | raund: %2 | %3 ms\n").arg(f.probablePrime?"EHTIMOL TUB":"MURAKKAB").arg(f.roundsRun).arg(tf,0,'f',6);
        text += QString("4. Miller-Rabin: %1 | raund: %2 | %3 ms\n").arg(m.probablePrime?"EHTIMOL TUB":"MURAKKAB").arg(m.roundsRun).arg(tm,0,'f',6);
        text += QString("5. Deterministik Miller-Rabin: %1 | %2 ms\n").arg(exact?"TUB":"TUB EMAS").arg(td,0,'f',6);
        text += QString("\nDeterministik algoritmlar mosligi: %1").arg((p1==p2 && p1==exact)?"MOS":"FARQ BOR");
        output->setPlainText(text);
    });
    window.show();
    return app.exec();
}
