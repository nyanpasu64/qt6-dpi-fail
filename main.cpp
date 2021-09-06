#include <QWidget>

#include <QLineEdit>
#include <QSpinBox>
#include <QVBoxLayout>

#include <QApplication>
#include <QDebug>

struct MySpinBox : QSpinBox {
    void stepBy(int steps) override {
        qDebug() << hasFocus();
        QSpinBox::stepBy(steps);
        if (!hasFocus()) {
            lineEdit()->deselect();
        }
    }
};


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget c;

    auto l = new QVBoxLayout;
    c.setLayout(l);

    {
        auto w = new MySpinBox;
        l->addWidget(w);
    }
    {
        auto w = new MySpinBox;
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
