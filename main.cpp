#include <QWidget>

#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>

#include <QApplication>
#include <QDebug>
#include <QMouseEvent>

class FindWidgetClicked : public QObject
{
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

int old_y = 0;

bool FindWidgetClicked::eventFilter(QObject *obj, QEvent *event)
{
    auto w = qobject_cast<QWidget *>(obj);
    if (!w) throw "up";

    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        auto p = QPoint(e->x(), old_y);
        old_y = e->y();

        while (auto child = w->childAt(p)) {
            w = child;
            p -= w->geometry().topLeft();
        }

        qDebug() << w;
        return true;
    } else {
        return QObject::eventFilter(obj, event);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget c;

    FindWidgetClicked filter;
    c.installEventFilter(&filter);

    auto l = new QVBoxLayout;
    c.setLayout(l);

    {
        auto w = new QListWidget;
        l->addWidget(w);
    }
    {
        auto w = new QPushButton;
        l->addWidget(w);
    }

    c.show();
    return a.exec();
}
