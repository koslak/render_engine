#ifndef RENDER_THREAD_H
#define RENDER_THREAD_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

class QImage;

class Render_thread : public QThread
{
    Q_OBJECT
public:
    Render_thread(QObject *parent = nullptr);
    ~Render_thread();

    void render(uint32_t image_width, uint32_t image_height);

signals:
    void rendered_image(const QImage &image, int progress);

protected:
    void run() override;

private:
    QMutex mutex;
    QWaitCondition wait_condition;

    bool is_restart{ false };
    bool is_abort{ false };

    uint32_t image_width{ 1 };
    uint32_t image_height{ 1 };
};

#endif // RENDER_THREAD_H
