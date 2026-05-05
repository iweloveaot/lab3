// polynomial_plot.h
#ifndef POLYNOMIAL_PLOT_H
#define POLYNOMIAL_PLOT_H

#include "polynomial.h"
#include <fstream>
#include <cmath>
#include <limits>
#include <algorithm>
#include <type_traits>

// Вспомогательная функция для вычисления значения многочлена для double
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, double>::type
evalPolynomialDouble(Polynomial<T>& poly, double x) {
    double result = 0.0;
    double power = 1.0;
        for (int i = 0; i <= poly.Degree(); i++) {
            result = result + static_cast<double>(poly[i]) * power;
            power = power * x; 
        }
    return result;
}

// Заглушка для нечисловых типов (комплексные числа, матрицы)
template<typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, double>::type
evalPolynomialDouble(Polynomial<T>&, double) {
    std::cerr << "Error: Plotting is only supported for numeric types (int, double)." << std::endl;
    return 0.0;
}

// Функция для построения графика многочлена и сохранения в SVG-файл
// Работает для типов T, поддерживающих арифметические операции и сравнение (int, double)
template<typename T>
void plotPolynomialToSVG(Polynomial<T>& poly, 
                         const std::string& filename,
                         double x_min = -10.0,
                         double x_max = 10.0,
                         int width = 800,
                         int height = 600,
                         int samples = 500) {
    
    // Открываем файл для записи
    std::ofstream svg(filename);
    if (!svg.is_open()) {
        throw std::runtime_error("Cannot open file for SVG output: " + filename);
    }

    // SVG заголовок и настройки
    svg << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
        << "width=\"" << width << "\" height=\"" << height << "\" "
        << "viewBox=\"0 0 " << width << " " << height << "\">\n";
    
    // Фон
    svg << "  <rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";
    
    // Вычисляем координатные преобразования
    // SVG: (0,0) в левом верхнем углу, Y растёт вниз
    // График: ось X горизонтальна, ось Y вертикальна вверх
    
    auto toScreenX = [x_min, x_max, width](double x) -> double {
        return (x - x_min) / (x_max - x_min) * width;
    };
    
    // Для Y нам нужно инвертировать направление и учесть диапазон значений
    // Сначала найдём диапазон Y для масштабирования
    double y_min = std::numeric_limits<double>::max();
    double y_max = std::numeric_limits<double>::lowest();
    
    for (int i = 0; i <= samples; ++i) {
        double x = x_min + (x_max - x_min) * i / samples;
        double y = evalPolynomialDouble(poly, x);
        
        if (y < y_min) y_min = y;
        if (y > y_max) y_max = y;
    }
    
    // Добавляем отступы по Y, чтобы график не прилипал к краям
    double y_range = y_max - y_min;
    if (y_range < 1.0) y_range = 1.0; // Минимальный диапазон
    y_min -= y_range * 0.1;
    y_max += y_range * 0.1;
    
    auto toScreenY = [y_min, y_max, height](double y) -> double {
        return height - (y - y_min) / (y_max - y_min) * height;
    };
    
    // Рисуем сетку (опционально)
    svg << "  <g stroke=\"#e0e0e0\" stroke-width=\"0.5\">\n";
    // Вертикальные линии сетки
    for (int i = 0; i <= 10; ++i) {
        double x = x_min + (x_max - x_min) * i / 10;
        double sx = toScreenX(x);
        svg << "    <line x1=\"" << sx << "\" y1=\"0\" x2=\"" << sx << "\" y2=\"" << height << "\"/>\n";
    }
    // Горизонтальные линии сетки
    for (int i = 0; i <= 10; ++i) {
        double y = y_min + (y_max - y_min) * i / 10;
        double sy = toScreenY(y);
        svg << "    <line x1=\"0\" y1=\"" << sy << "\" x2=\"" << width << "\" y2=\"" << sy << "\"/>\n";
    }
    svg << "  </g>\n";
    
    // Рисуем оси координат
    svg << "  <g stroke=\"#000000\" stroke-width=\"2\">\n";
    // Ось X (y=0)
    if (y_min <= 0 && 0 <= y_max) {
        double sy = toScreenY(0);
        svg << "    <line x1=\"0\" y1=\"" << sy << "\" x2=\"" << width << "\" y2=\"" << sy << "\"/>\n";
    }
    // Ось Y (x=0)
    if (x_min <= 0 && 0 <= x_max) {
        double sx = toScreenX(0);
        svg << "    <line x1=\"" << sx << "\" y1=\"0\" x2=\"" << sx << "\" y2=\"" << height << "\"/>\n";
    }
    svg << "  </g>\n";
    
    // Рисуем график многочлена
    svg << "  <polyline fill=\"none\" stroke=\"#0066cc\" stroke-width=\"2\" points=\"";
    
    bool first = true;
    for (int i = 0; i <= samples; ++i) {
        double x = x_min + (x_max - x_min) * i / samples;
        double y = evalPolynomialDouble(poly, x);
        
        double sx = toScreenX(x);
        double sy = toScreenY(y);
        
        if (first) {
            svg << sx << "," << sy;
            first = false;
        } else {
            svg << " " << sx << "," << sy;
        }
    }
    svg << "\"/>\n";
    
    // Подписи осей
    svg << "  <g font-family=\"Arial, sans-serif\" font-size=\"12\" fill=\"#333\">\n";
    // Подпись оси X
    svg << "    <text x=\"" << (width - 40) << "\" y=\"" << (toScreenY(0) - 5) << "\">X</text>\n";
    // Подпись оси Y
    svg << "    <text x=\"" << (toScreenX(0) + 5) << "\" y=\"20\">Y</text>\n";
    // Значения на осях
    svg << "    <text x=\"" << (toScreenX(x_min) + 2) << "\" y=\"" << (height - 5) << "\">" << x_min << "</text>\n";
    svg << "    <text x=\"" << (toScreenX(x_max) - 20) << "\" y=\"" << (height - 5) << "\">" << x_max << "</text>\n";
    svg << "  </g>\n";
    
    // Заголовок графика
    svg << "  <text x=\"" << (width/2) << "\" y=\"25\" font-family=\"Arial, sans-serif\" "
        << "font-size=\"14\" font-weight=\"bold\" text-anchor=\"middle\" fill=\"#000\">";
    svg << "Polynomial of degree " << poly.Degree() << "</text>\n";
    
    // Закрытие SVG
    svg << "</svg>\n";
    svg.close();
    
    std::cout << "Graph saved to: " << filename << std::endl;
}

#endif // POLYNOMIAL_PLOT_H