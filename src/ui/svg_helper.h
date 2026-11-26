#pragma once

#include <QColor>
#include <QIcon>
#include <QPixmap>
#include <QSize>
#include <QString>

namespace carent::ui {

[[nodiscard]] QPixmap colorizeSvgPixmap(
    const QString &resourcePath,
    const QColor &color,
    const QSize &size = QSize(20, 20));

[[nodiscard]] QIcon colorizeSvgIcon(
    const QString &resourcePath,
    const QColor &color,
    const QSize &size = QSize(20, 20));

} // namespace carent::ui