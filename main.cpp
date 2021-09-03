#include <QWidget>

#include <QVBoxLayout>

#include <QApplication>
#include <QDebug>
#include <QListWidget>
#include <QMouseEvent>

class FindWidgetClicked : public QObject
{
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

bool FindWidgetClicked::eventFilter(QObject *obj, QEvent *event)
{
    auto w = qobject_cast<QWidget *>(obj);
    if (!w) throw "up";

    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        auto p = QPoint(e->x(), e->y());

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

    c.show();
    return a.exec();
}
