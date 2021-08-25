#include <QPushButton>
#include <QWidget>

#include <QVBoxLayout>

#include <QApplication>
#include <QDebug>
#include <QProxyStyle>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget c;

    auto l = new QVBoxLayout;
    c.setLayout(l);

    {
        auto w = new QPushButton("QProxyStyle(QApplication::style())");
        l->addWidget(w);

        QObject::connect(w, &QPushButton::pressed, w, [w]() {
            (QProxyStyle(QApplication::style()));
        });
    }

    {
        auto w = new QPushButton("QProxyStyle(w->style())");
        l->addWidget(w);

        QObject::connect(w, &QPushButton::pressed, w, [w]() {
            QProxyStyle(w->style());
        });
    }

    c.show();
    return a.exec();
}
