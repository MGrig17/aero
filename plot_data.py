import matplotlib.pyplot as plt
import numpy as np
import re
from datetime import datetime
import os


class DataAnalyzer:
    def __init__(self, filename="data.txt"):
        self.filename = filename
        self.data = []

    def parse_data(self):
        """Парсинг данных из файла"""
        if not os.path.exists(self.filename):
            print(f"Файл {self.filename} не найден!")
            return False

        self.data = []
        angle_data = []
        weight1_data = []
        weight2_data = []
        time_data = []

        print("Чтение данных из файла...")

        with open(self.filename, 'r', encoding='utf-8') as file:
            lines = file.readlines()

        for line in lines:
            # Ищем строки с измерениями (формат: угол, вес1, вес2)
            if '°' in line and 'g' in line:
                # Удаляем временные метки если есть
                clean_line = re.sub(r'\[\d+:\d+:\d+\]\s*ARDUINO:\s*', '', line.strip())

                # Парсим данные измерений
                # Форматы: "0.0° 1.23g 2.45g" или "0s 0.0° 1.23g 2.45g"
                parts = clean_line.split()

                try:
                    # Обрабатываем разные форматы
                    if 's' in parts[0]:  # Если есть время в секундах
                        time_val = float(parts[0].replace('s', ''))
                        angle_val = float(parts[1].replace('°', ''))
                        weight1_val = float(parts[2].replace('g', ''))
                        weight2_val = float(parts[3].replace('g', ''))
                    else:  # Если нет времени
                        angle_val = float(parts[0].replace('°', ''))
                        weight1_val = float(parts[1].replace('g', ''))
                        weight2_val = float(parts[2].replace('g', ''))
                        time_val = len(time_data)  # Авто-время

                    angle_data.append(angle_val)
                    weight1_data.append(weight1_val)
                    weight2_data.append(weight2_val)
                    time_data.append(time_val)

                except (ValueError, IndexError) as e:
                    continue  # Пропускаем некорректные строки

        self.data = {
            'time': time_data,
            'angle': angle_data,
            'weight1': weight1_data,
            'weight2': weight2_data
        }

        print(f"Найдено {len(angle_data)} записей измерений")
        return True

    def plot_angle_vs_weights(self):
        """График зависимости весов от угла"""
        if not self.data:
            print("Нет данных для построения графика!")
            return

        fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10))

        # График 1: Веса от угла
        ax1.plot(self.data['angle'], self.data['weight1'], 'b-', label='Вес 1', linewidth=2, marker='o', markersize=4)
        ax1.plot(self.data['angle'], self.data['weight2'], 'r-', label='Вес 2', linewidth=2, marker='s', markersize=4)
        ax1.set_xlabel('Угол (°)')
        ax1.set_ylabel('Вес (g)')
        ax1.set_title('Зависимость весов от угла')
        ax1.legend()
        ax1.grid(True, alpha=0.3)

        # График 2: Веса от времени
        ax2.plot(self.data['time'], self.data['weight1'], 'b-', label='Вес 1', linewidth=2)
        ax2.plot(self.data['time'], self.data['weight2'], 'r-', label='Вес 2', linewidth=2)
        ax2.set_xlabel('Время (с)')
        ax2.set_ylabel('Вес (g)')
        ax2.set_title('Изменение весов во времени')
        ax2.legend()
        ax2.grid(True, alpha=0.3)

        plt.tight_layout()
        plt.show()

    def plot_3d(self):
        """3D график угол-время-вес"""
        if not self.data or len(self.data['angle']) < 3:
            print("Недостаточно данных для 3D графика!")
            return

        from mpl_toolkits.mplot3d import Axes3D

        fig = plt.figure(figsize=(12, 8))
        ax = fig.add_subplot(111, projection='3d')

        # Создаем сетку для поверхности
        angles = np.array(self.data['angle'])
        times = np.array(self.data['time'])
        weights1 = np.array(self.data['weight1'])
        weights2 = np.array(self.data['weight2'])

        # Точечный график
        scatter1 = ax.scatter(angles, times, weights1, c=weights1, cmap='viridis',
                              label='Вес 1', s=50, alpha=0.7)
        scatter2 = ax.scatter(angles, times, weights2, c=weights2, cmap='plasma',
                              label='Вес 2', s=50, alpha=0.7, marker='^')

        ax.set_xlabel('Угол (°)')
        ax.set_ylabel('Время (с)')
        ax.set_zlabel('Вес (g)')
        ax.set_title('3D зависимость: Угол-Время-Вес')
        ax.legend()

        plt.show()

    def plot_individual(self):
        """Отдельные графики для каждого датчика"""
        if not self.data:
            print("Нет данных для построения графика!")
            return

        fig, axes = plt.subplots(2, 2, figsize=(15, 10))

        # График угла
        axes[0, 0].plot(self.data['time'], self.data['angle'], 'g-', linewidth=2)
        axes[0, 0].set_xlabel('Время (с)')
        axes[0, 0].set_ylabel('Угол (°)')
        axes[0, 0].set_title('Изменение угла во времени')
        axes[0, 0].grid(True, alpha=0.3)

        # График веса 1
        axes[0, 1].plot(self.data['time'], self.data['weight1'], 'b-', linewidth=2)
        axes[0, 1].set_xlabel('Время (с)')
        axes[0, 1].set_ylabel('Вес 1 (g)')
        axes[0, 1].set_title('Датчик веса 1')
        axes[0, 1].grid(True, alpha=0.3)

        # График веса 2
        axes[1, 0].plot(self.data['time'], self.data['weight2'], 'r-', linewidth=2)
        axes[1, 0].set_xlabel('Время (с)')
        axes[1, 0].set_ylabel('Вес 2 (g)')
        axes[1, 0].set_title('Датчик веса 2')
        axes[1, 0].grid(True, alpha=0.3)

        # Сравнение весов
        axes[1, 1].plot(self.data['time'], self.data['weight1'], 'b-', label='Вес 1', linewidth=2)
        axes[1, 1].plot(self.data['time'], self.data['weight2'], 'r-', label='Вес 2', linewidth=2)
        axes[1, 1].set_xlabel('Время (с)')
        axes[1, 1].set_ylabel('Вес (g)')
        axes[1, 1].set_title('Сравнение датчиков веса')
        axes[1, 1].legend()
        axes[1, 1].grid(True, alpha=0.3)

        plt.tight_layout()
        plt.show()

    def show_statistics(self):
        """Показать статистику данных"""
        if not self.data:
            print("Нет данных для статистики!")
            return

        print("\n" + "=" * 50)
        print("СТАТИСТИКА ДАННЫХ")
        print("=" * 50)
        print(f"Всего измерений: {len(self.data['angle'])}")
        print(f"Диапазон углов: {min(self.data['angle']):.1f}° - {max(self.data['angle']):.1f}°")
        print(f"Диапазон веса 1: {min(self.data['weight1']):.2f}g - {max(self.data['weight1']):.2f}g")
        print(f"Диапазон веса 2: {min(self.data['weight2']):.2f}g - {max(self.data['weight2']):.2f}g")
        print(f"Средний вес 1: {np.mean(self.data['weight1']):.2f}g")
        print(f"Средний вес 2: {np.mean(self.data['weight2']):.2f}g")
        print("=" * 50)


def main():
    analyzer = DataAnalyzer("data.txt")

    if not analyzer.parse_data():
        return

    analyzer.show_statistics()

    while True:
        print("\nВыберите тип графика:")
        print("1 - Зависимость весов от угла")
        print("2 - Отдельные графики датчиков")
        print("3 - 3D график (требует больше данных)")
        print("4 - Показать статистику")
        print("0 - Выход")

        choice = input("Ваш выбор: ").strip()

        if choice == '1':
            analyzer.plot_angle_vs_weights()
        elif choice == '2':
            analyzer.plot_individual()
        elif choice == '3':
            analyzer.plot_3d()
        elif choice == '4':
            analyzer.show_statistics()
        elif choice == '0':
            break
        else:
            print("Неверный выбор!")


if __name__ == "__main__":
    main()