#include <QApplication>
#include <QWidget>

#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QMetaEnum>

template<typename EnumT>
QString stringify(const EnumT value)
{
  return QMetaEnum::fromType<EnumT>().valueToKey((int) value);
}

int main(int argc, char *argv[])
{
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::RoundPreferFloor
    );
    qDebug() << QGuiApplication::highDpiScaleFactorRoundingPolicy();
    QApplication a(argc, argv);
    QWidget c;

    auto l = new QVBoxLayout;
    c.setLayout(l);
    l->setSizeConstraint(QLayout::SetFixedSize);

    auto w = new QPushButton("Query devicePixelRatio()");
    l->addWidget(w);
    w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    c.show();

    QObject::connect(w, &QPushButton::pressed, w, [w]() {
        w->setText(QString("%1 -> %2")
            .arg(stringify(QGuiApplication::highDpiScaleFactorRoundingPolicy()))
            .arg(w->devicePixelRatio())
        );
    });

    return a.exec();
}
