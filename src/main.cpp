#include <SFML/Graphics.hpp>
#include <complex>
#include <omp.h>

int mandelbrot(double real, double imag, int maxIterations) {
    double zReal = 0.0, zImag = 0.0; // Основные переменные
    double zRealSq = 0.0, zImagSq = 0.0; // Квадраты реальной и мнимой частей
    double zRealFast = zReal, zImagFast = zImag; // "быстрый указатель"
    int iter = 0;

    while (zRealSq + zImagSq <= 4.0 && iter < maxIterations) {
        // Раскрытие Z^2 + c = (x+iy)^2 +(x1+iy1) = x^2+2xiy-y^2 + x1 + y1
        zImag = 2.0 * zReal * zImag + imag;
        zReal = zRealSq - zImagSq + real;

        // Обновление квадратов заранее
        zRealSq = zReal * zReal;
        zImagSq = zImag * zImag;

        // Обновление быстрого указателя
        // Если элементы цикличны, то они входят в множество
        // к примеру для c = -1, значения будут равны либо 0 либо -1, что указывает на то, 
        // что точка не покидает окружность
        // что позволят быстро считать "чёрные" точки
        if (iter % 2 == 0) {
            zImagFast = 2.0 * zRealFast * zImagFast + imag;
            zRealFast = zRealFast * zRealFast - zImagFast * zImagFast + real;
            // Проверка на наличие цикла
            if (zReal == zRealFast && zImag == zImagFast) {
                return maxIterations; // Точка в пределах множества
            }
        }

        iter++;
    }
    return iter; // Возвращаем количество итераций
}


sf::Color getColor(int iterations, int maxIterations) {
    if (iterations == maxIterations) {
        return sf::Color(0, 0, 0); // Черный цвет для точек внутри множества
    }

    // Нормализуем количество итераций
    // Для понимания на сколько отдалилась точка
    double t = static_cast<double>(iterations) / maxIterations;

    // Задаем цвета для градиента
    // чем больше t - тем больше итерций понадобилось на поиск её "невходимости" в множество (преобретает синий цвет на максимальном значении)
    int r = static_cast<int>(9 * (1 - t) * pow(t,3) * 255);
    int g = static_cast<int>(15 * pow((1 - t),2) * pow(t,2) * 255);
    int b = static_cast<int>(8.5 * pow((1 - t),3) * t * 255);
    return sf::Color(r, g, b);
}

int main() {
    const int width = 800;
    const int height = 600;
    const int maxIterations = 100;

    // Создаем окно
    sf::RenderWindow window(sf::VideoMode(width, height), "Mandelbrot Set with Keyboard Controls");

    // Переменные для масштабирования и смещения
    double zoom = 1.0;
    double offsetX = 0.0;
    double offsetY = 0.0;

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


//цикл по точкам (пикселям)
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            // Преобразование координат под число c
            double real = (x - width / 2.0) * 4.0 / (width * zoom) + offsetX;  
            double imag = (y - height / 2.0) * 4.0 / (height * zoom) + offsetY;

            int n = mandelbrot(real, imag, maxIterations);
            
            // Получаем цвет в зависимости от количества итераций
            sf::Color color = getColor(n, maxIterations);
            
            pixels[(x + y * width) * 4 + 0] = color.r; // Red
            pixels[(x + y * width) * 4 + 1] = color.g; // Green
            pixels[(x + y * width) * 4 + 2] = color.b; // Blue
        }
    }

    // Обновляем текстуру
    texture.update(pixels);
    delete[] pixels; 
    needsUpdate = false; 
}
 // Освобождаем выделенную память
        window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
    }
    return 0;
}