#include "windowblur.h"
#include <QPainterPath>
#include <KWindowEffects>

WindowBlur::WindowBlur(QObject *parent) noexcept
    : QObject(parent)
    , m_view(nullptr)
    , m_enabled(false)
    , m_windowRadius(0.0)
{
}

WindowBlur::~WindowBlur()
{
}

void WindowBlur::classBegin()
{
}

void WindowBlur::componentComplete()
{
    updateBlur();
}

void WindowBlur::setView(QWindow *view)
{
    if (view != m_view) {
        m_view = view;
        updateBlur();
        emit viewChanged();

        connect(m_view, &QWindow::visibleChanged, this, &WindowBlur::onViewVisibleChanged);
    }
}

QWindow* WindowBlur::view() const
{
    return m_view;
}

void WindowBlur::setGeometry(const QRect &rect)
{
    if (rect != m_rect) {
        m_rect = rect;
        updateBlur();
        emit geometryChanged();
    }
}

QRect WindowBlur::geometry() const
{
    return m_rect;
}

void WindowBlur::setEnabled(bool enabled)
{
    if (enabled != m_enabled) {
        m_enabled = enabled;
        updateBlur();
        emit enabledChanged();
    }
}

bool WindowBlur::enabled() const
{
    return m_enabled;
}

void WindowBlur::setWindowRadius(qreal radius)
{
    if (radius != m_windowRadius) {
        m_windowRadius = radius;
        updateBlur();
        emit windowRadiusChanged();
    }
}

qreal WindowBlur::windowRadius() const
{
    return m_windowRadius;
}

void WindowBlur::onViewVisibleChanged(bool visible)
{
    if (visible)
        updateBlur();
}

void WindowBlur::updateBlur()
{
    if (m_view) {
        if (m_enabled) {
            QRect rect = QRect(0, 0, m_rect.width(), m_rect.height());
            QPainterPath path;
            rect.adjust(1, 1, -1, -1);
            path.addRoundedRect(rect, m_windowRadius, m_windowRadius);
            KWindowEffects::enableBlurBehind(m_view->winId(), true, path.toFillPolygon().toPolygon());
        } else {
            KWindowEffects::enableBlurBehind(m_view->winId(), false);
        }
    }
}