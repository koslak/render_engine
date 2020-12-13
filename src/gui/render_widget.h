#ifndef RENDER_WIDGET_H
#define RENDER_WIDGET_H

#include <QWidget>

class Render_widget : public QWidget
{
    Q_OBJECT
public:
    explicit Render_widget(QWidget *parent = nullptr);

public slots:
    void refresh() noexcept;

signals:
    void renderer_progress(const int &step);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    std::unique_ptr<QImage> image;

    uint32_t width_image;
    uint32_t height_image;
    int render_progress{ 0 };
};

#endif // RENDER_WIDGET_H
