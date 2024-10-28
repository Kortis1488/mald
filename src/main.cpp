#include <SFML/Graphics.hpp>
#include <complex>
#include <omp.h>

int mandelbrot(double real, double imag, int maxIterations) {
    double zReal = 0.0;
    double zImag = 0.0;
    double zRealSq = 0.0;
    double zImagSq = 0.0;

    int iterations = 0;
    double zRealSlow = zReal, zImagSlow = zImag; // "Черепаха"
    double zRealFast = zReal, zImagFast = zImag; // "Заяц"

    while (zRealSq + zImagSq <= 4.0 && iterations < maxIterations) {
        zImag = 2.0 * zReal * zImag + imag;
        zReal = zRealSq - zImagSq + real;

        zRealSq = zReal * zReal;
        zImagSq = zImag * zImag;

        // Обновляем позиции "черепахи" и "зайца"
        if (iterations % 2 == 0) { // Каждые две итерации "заяц" обновляется
            zImagFast = 2.0 * zRealFast * zImagFast + imag;
            zRealFast = zRealFast * zRealFast - zImagFast * zImagFast + real;

            if (zReal == zRealFast && zImag == zImagFast) {
                // Если позиции совпадают, точка зациклилась
                return maxIterations;
            }
        }

        iterations++;
    }

    return iterations;
}


sf::Color getColor(int iterations, int maxIterations) {
    if (iterations == maxIterations) {
        return sf::Color(0, 0, 0); // Черный цвет для точек внутри множества
    }

    // Нормализуем количество итераций
    double t = static_cast<double>(iterations) / maxIterations;

    // Задаем цвета для градиента
    int r = static_cast<int>(9 * (1 - t) * t * t * t * 255);
    int g = static_cast<int>(15 * (1 - t) * (1 - t) * t * t * 255);
    int b = static_cast<int>(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);

    return sf::Color(r, g, b);
}

int main() {
    const int width = 800;
    const int height = 600;
    const int maxIterations = 100;
    std::vector<int> iterat;

    // Создаем окно
    sf::RenderWindow window(sf::VideoMode(width, height), "Mandelbrot Set with Keyboard Controls");

    // Переменные для масштабирования и смещения
    double zoom = 1.0;
    double offsetX = 0.0;
    double offsetY = 0.0;

    // Создаем текстуру и спрайт
    sf::Texture texture;
    texture.create(width, height);
    sf::Sprite sprite(texture);

    // Основной цикл программы
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

       bool needsUpdate = true; // Флаг обновления

// Изменяем обработку ввода
if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    zoom *= 1.05; // Уменьшение масштаба (приближение)
    needsUpdate = true; // Пометить для обновления
}
if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    zoom *= 0.95; // Увеличение масштаба (удаление)
    needsUpdate = true; // Пометить для обновления
}

// Обработка перемещения с помощью стрелок
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    offsetX -= 0.1 / zoom; // Перемещение влево
    needsUpdate = true; // Пометить для обновления
}
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    offsetX += 0.1 / zoom; // Перемещение вправо
    needsUpdate = true; // Пометить для обновления
}
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    offsetY -= 0.1 / zoom; // Перемещение вверх
    needsUpdate = true; // Пометить для обновления
}
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
    offsetY += 0.1 / zoom; // Перемещение вниз
    needsUpdate = true; // Пометить для обновления
}

// Заполняем текстуру значениями множества Мандельброта только при необходимости
if (needsUpdate) {
    sf::Uint8* pixels = new sf::Uint8[width * height * 4];

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            double real = (x - width / 2.0) * 4.0 / (width * zoom) + offsetX;  // Преобразование координат
            double imag = (y - height / 2.0) * 4.0 / (height * zoom) + offsetY;

            int n = mandelbrot(real, imag, maxIterations);
            
            // Получаем цвет в зависимости от количества итераций
            sf::Color color = getColor(n, maxIterations);
            
            pixels[(x + y * width) * 4 + 0] = color.r; // Red
            pixels[(x + y * width) * 4 + 1] = color.g; // Green
            pixels[(x + y * width) * 4 + 2] = color.b; // Blue
            pixels[(x + y * width) * 4 + 3] = 255;     // Alpha
        }
    }

    // Обновляем текстуру
    texture.update(pixels);
    delete[] pixels; // Освобождаем выделенную память
    needsUpdate = false; // Сбросить флаг
}
 // Освобождаем выделенную память

        // Очищаем окно
        window.clear(sf::Color::Black);

        // Рисуем спрайт с текстурой
        window.draw(sprite);

        // Отображаем содержимое окна
        window.display();
    }

    return 0;
}
