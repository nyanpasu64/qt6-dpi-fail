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

bool FindWidgetClicked::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        qDebug() << QApplication::widgetAt(e->globalPos());
        return true;
    } else {
        return QObject::eventFilter(obj, event);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FindWidgetClicked filter;
    a.installEventFilter(&filter);

    QWidget c;

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
