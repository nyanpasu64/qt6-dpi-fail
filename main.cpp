#include <QApplication>
#include <QWidget>

#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QMetaEnum>
#include <QScreen>
#include <QWindow>

/// Emits QWindow::screenChanged() for a window and all (MDI?) child windows.
static void recursive_screenChanged(QWindow * win) {
    QScreen *screen = win->screen();
    emit win->screenChanged(screen);
    for (QObject *child : win->children()) {
        if (child->isWindowType())
            recursive_screenChanged(static_cast<QWindow *>(child));
    }
}

/// Hook a QScreen to emit QWindow::screenChanged() for all windows within the screen,
/// whenever the QScreen changes DPI.
static void emit_screenChanged_upon_dpi_changed(QScreen * screen) {
    // We can't emit QWindow::screenChanged() too early,
    // since it only works after QScreenPrivate::updateGeometriesWithSignals() and
    // QGuiApplicationPrivate::resetCachedDevicePixelRatio() have been called.
    // So use a QueuedConnection.
    //
    // Unfortunately we can't use a UniqueConnection:
    // (https://doc.qt.io/qt-5/qobject.html#connect)
    // > Qt::UniqueConnections do not work for lambdas, non-member functions
    // > and functors; they only apply to connecting to member functions.
    //
    // So far I've only seen each QScreen get QGuiApplication::screenAdded() once,
    // but just in case it happens multiple times, ensure we only connect once.
    static constexpr char const* SCREEN_DPI_CONNECTED = "exo_dpi_connected";
    if (screen->property(SCREEN_DPI_CONNECTED).toBool()) {
        return;
    }
    screen->setProperty(SCREEN_DPI_CONNECTED, true);

    QObject::connect(
        screen, &QScreen::logicalDotsPerInchChanged,
        screen, [screen] {
            const auto allWindows = QGuiApplication::allWindows();
            for (QWindow * window : allWindows) {
                if (!window->isTopLevel() || window->screen() != screen)
                    continue;
                recursive_screenChanged(window);
            }
        },
        Qt::QueuedConnection);

    // It may be worth emitting QWindow::screenChanged()
    // on other QScreen::fooChanged() properties as well.
    // This will fix applications which only pick up resolution changes
    // upon QWindow::screenChanged().
    // However it's not necessary to fix Qt Widgets itself.
};

/// Hook all current and future QScreen objects
/// so they emit QWindow::screenChanged whenever the screen's logical DPI changes.
/// This is a workaround for a bug (https://bugreports.qt.io/browse/QTBUG-95925)
/// where QWidget only relayouts when changing QScreen,
/// not when a QScreen's DPI changes.
static void hook_all_screens(QGuiApplication & app) {
    // Note that we don't need to emit QWindow::screenChanged right away,
    // only when the DPI property changes.

    // Hook all existing QScreen.
    auto screens = QGuiApplication::screens();
    for (QScreen * screen : qAsConst(screens)) {
        emit_screenChanged_upon_dpi_changed(screen);
    }

    // Hook every QScreen added later.
    // This *should* not hook the same QScreen twice,
    // since every QScreen gets screenAdded() exactly once in my testing.
    QObject::connect(
        &app, &QGuiApplication::screenAdded,
        &app, emit_screenChanged_upon_dpi_changed);
}

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
    hook_all_screens(a);

    QWidget c;

    auto l = new QVBoxLayout;
    c.setLayout(l);
    l->setSizeConstraint(QLayout::SetFixedSize);

    auto w = new QPushButton("IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII");
    l->addWidget(w);
    w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    c.show();

    QObject::connect(w, &QPushButton::pressed, w, [w]() {
        w->setText(QString("%1 -> %2")
            .arg(stringify(QGuiApplication::highDpiScaleFactorRoundingPolicy()))
            .arg(w->devicePixelRatio())
        );
        qDebug() << w->sizeHint();
    });

    return a.exec();
}
