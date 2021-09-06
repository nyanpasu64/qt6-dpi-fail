#include <QWidget>

#include <QLineEdit>
#include <QSpinBox>
#include <QVBoxLayout>

#include <QApplication>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget c;

    auto l = new QVBoxLayout;
    c.setLayout(l);

    {
        auto w = new QSpinBox;
        w->setFocusPolicy(Qt::StrongFocus);
        l->addWidget(w);
    }
    {
        auto w = new QSpinBox;
        w->setFocusPolicy(Qt::StrongFocus);
        l->addWidget(w);
    }
    {
        auto w = new QLineEdit;
        l->addWidget(w);
    }

    c.show();
    return a.exec();
}
