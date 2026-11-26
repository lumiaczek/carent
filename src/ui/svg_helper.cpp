#include "svg_helper.h"
#include <QPainter>

namespace carent::ui {

QPixmap colorizeSvgPixmap(const QString &resourcePath, const QColor &color, const QSize &size) {
    QPixmap pixmap = QIcon(resourcePath).pixmap(size);
    if (pixmap.isNull()) {
        return pixmap;
    }

    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(pixmap.rect(), color);
    painter.end();

    return pixmap;
}

QIcon colorizeSvgIcon(const QString &resourcePath, const QColor &color, const QSize &size) {
    QPixmap pixmap = colorizeSvgPixmap(resourcePath, color, size);
    return pixmap.isNull() ? QIcon() : QIcon(pixmap);
}

} // namespace carent::ui