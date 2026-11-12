#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QHBoxLayout>
#include <chrono>
#include "prime_checker.h"
#include "probabilistic_primality.h"

template<class F> double elapsedMs(F fn) {
    auto start=std::chrono::steady_clock::now();
    fn();
    return std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-start).count();
}

int main(int argc,char *argv[]) {
    QApplication app(argc,argv);
    QWidget window;
    window.setWindowTitle("Tub sonlar laboratoriyasi");
    window.resize(820,620);
    window.setStyleSheet("QWidget{font-size:14px;} QLabel#title{font-size:22px;font-weight:700;color:#2563eb;} QPushButton{padding:10px;border-radius:6px;background:#2563eb;color:white;} QPushButton:hover{background:#1d4ed8;} QLineEdit,QPlainTextEdit{padding:8px;border:1px solid #9ca3af;border-radius:5px;}");
    auto *layout=new QVBoxLayout(&window);
    auto *title=new QLabel("TUB SONLARNI TAHLIL QILISH");
    title->setObjectName("title");
    layout->addWidget(title);
    layout->addWidget(new QLabel("0 dan UINT64_MAX gacha butun son kiriting:"));
    auto *input=new QLineEdit;
    input->setPlaceholderText("Masalan: 1000003");
    layout->addWidget(input);
    auto *buttons=new QHBoxLayout;
    auto *run=new QPushButton("Tekshirish");
    auto *save=new QPushButton("CSV hisobotni saqlash");
    auto *clear=new QPushButton("Tozalash");
    buttons->addWidget(run); buttons->addWidget(save); buttons->addWidget(clear);
    layout->addLayout(buttons);
    auto *output=new QPlainTextEdit;
    output->setReadOnly(true);
    output->setPlaceholderText("Algoritmlar natijalari shu yerda ko'rsatiladi...");
    layout->addWidget(output);
    auto *note=new QLabel("Fermat va tasodifiy Miller-Rabin ehtimollik testlari. Deterministik Miller-Rabin uint64 diapazonida aniq natija beradi. 10^12 dan katta sonlarda sekin sinov-bo'linish algoritmlari o'tkazib yuboriladi.");
    note->setWordWrap(true); layout->addWidget(note);
    QString latestCsv;
    QObject::connect(run,&QPushButton::clicked,[&](){
        bool ok=false;
        const QString raw=input->text().trimmed();
        const qulonglong parsed=raw.toULongLong(&ok,10);
        if(!ok||raw.isEmpty()){
            QMessageBox::warning(&window,"Kiritish xatosi","0 dan UINT64_MAX gacha butun son kiriting.");
            return;
        }
        const uint64_t n=static_cast<uint64_t>(parsed);
        QString result=QString("Kiritilgan son: %1\n\n").arg(QString::number(n));
        latestCsv="number,algorithm,result,checks_or_rounds,time_ms\n";
        latestCsv+=QString::number(n)+",";
        if(n<=1000000000000ULL){
            uint64_t divisor=0,checks=0; bool prime=false;
            double ms=elapsedMs([&]{prime=isPrime(n,divisor,checks);});
            result+=QString("1. Oddiy bo'linish: %1 | tekshiruv: %2 | %3 ms\n").arg(prime?"TUB":"TUB EMAS").arg(checks).arg(ms,0,'f',6);
            latestCsv+=QString("trial_division,%1,%2,%3\n").arg(prime?"prime":"composite").arg(checks).arg(ms,0,'f',6);
            divisor=checks=0;
            ms=elapsedMs([&]{prime=isPrime6k1(n,divisor,checks);});
            result+=QString("2. 6k +/- 1: %1 | tekshiruv: %2 | %3 ms\n").arg(prime?"TUB":"TUB EMAS").arg(checks).arg(ms,0,'f',6);
            latestCsv+=QString("six_k_plus_minus_one,%1,%2,%3\n").arg(prime?"prime":"composite").arg(checks).arg(ms,0,'f',6);
        } else {
            result+="1. Oddiy bo'linish: katta son sabab o'tkazib yuborildi\n";
            result+="2. 6k +/- 1: katta son sabab o'tkazib yuborildi\n";
            latestCsv+="trial_division,skipped_large_input,NA,NA\nsix_k_plus_minus_one,skipped_large_input,NA,NA\n";
        }
        ProbablePrimeResult f,m;
        double ms=elapsedMs([&]{f=fermatTest(n,10,2026);});
        result+=QString("3. Fermat (10 raund): %1 | raund: %2 | %3 ms\n").arg(f.probablePrime?"EHTIMOL TUB":"MURAKKAB").arg(f.roundsRun).arg(ms,0,'f',6);
        latestCsv+=QString("fermat,%1,%2,%3\n").arg(f.probablePrime?"probable_prime":"composite").arg(f.roundsRun).arg(ms,0,'f',6);
        ms=elapsedMs([&]{m=millerRabinTest(n,10,2026);});
        result+=QString("4. Miller-Rabin (10 raund): %1 | raund: %2 | %3 ms\n").arg(m.probablePrime?"EHTIMOL TUB":"MURAKKAB").arg(m.roundsRun).arg(ms,0,'f',6);
        latestCsv+=QString("miller_rabin,%1,%2,%3\n").arg(m.probablePrime?"probable_prime":"composite").arg(m.roundsRun).arg(ms,0,'f',6);
        bool exact=false; ms=elapsedMs([&]{exact=isPrimeDeterministic64(n);});
        result+=QString("5. Deterministik Miller-Rabin: %1 | %2 ms\n").arg(exact?"TUB":"TUB EMAS").arg(ms,0,'f',6);
        latestCsv+=QString("deterministic_miller_rabin_64,%1,7,%2\n").arg(exact?"prime":"composite").arg(ms,0,'f',6);
        result+=QString("\nDeterministik algoritmlar mosligi: %1").arg(n<=1000000000000ULL && (exact==isPrime6k1(n,*new uint64_t(0),*new uint64_t(0)))?"MOS":"Natija deterministik MR bilan tasdiqlandi");
        output->setPlainText(result);
    });
    QObject::connect(save,&QPushButton::clicked,[&](){
        if(latestCsv.isEmpty()){QMessageBox::information(&window,"Hisobot","Avval sonni tekshiring.");return;}
        const QString path=QFileDialog::getSaveFileName(&window,"CSV hisobotni saqlash","prime_report.csv","CSV (*.csv)");
        if(path.isEmpty())return;
        QFile file(path);
        if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){QMessageBox::critical(&window,"Xato","Faylni yozib bo'lmadi.");return;}
        QTextStream stream(&file); stream<<latestCsv; file.close();
        QMessageBox::information(&window,"Tayyor","CSV hisobot saqlandi.");
    });
    QObject::connect(clear,&QPushButton::clicked,[&](){input->clear();output->clear();latestCsv.clear();input->setFocus();});
    window.show();
    return app.exec();
}
