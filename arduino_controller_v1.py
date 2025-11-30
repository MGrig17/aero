import serial
import threading
import time
import sys
import os


class ArduinoSerialController:
    def __init__(self, port=None, baudrate=9600):
        self.port = port
        self.baudrate = baudrate
        self.ser = None
        self.running = False
        self.read_thread = None

    def list_ports(self):
        """Список доступных COM портов"""
        import serial.tools.list_ports
        ports = serial.tools.list_ports.comports()
        available_ports = []
        print("Доступные COM порты:")
        for i, port in enumerate(ports):
            print(f"{i + 1}. {port.device} - {port.description}")
            available_ports.append(port.device)
        return available_ports

    def connect(self, port=None):
        """Подключение к Arduino"""
        if port:
            self.port = port

        if not self.port:
            available_ports = self.list_ports()
            if not available_ports:
                print("Не найдено доступных COM портов!")
                return False
            self.port = available_ports[0]
            print(f"Автоматически выбран порт: {self.port}")

        try:
            self.ser = serial.Serial(
                port=self.port,
                baudrate=self.baudrate,
                timeout=1,
                writeTimeout=1
            )
            time.sleep(2)  # Ждем инициализации Arduino
            print(f"Успешно подключено к {self.port}")
            return True
        except Exception as e:
            print(f"Ошибка подключения к {self.port}: {e}")
            return False

    def start_reading(self):
        """Запуск потока чтения"""
        self.running = True
        self.read_thread = threading.Thread(target=self._read_serial)
        self.read_thread.daemon = True
        self.read_thread.start()

    def _read_serial(self):
        """Поток для чтения данных из Serial"""
        while self.running and self.ser and self.ser.is_open:
            try:
                if self.ser.in_waiting > 0:
                    line = self.ser.readline().decode('utf-8', errors='ignore').strip()
                    if line:
                        print(f"Arduino: {line}")
            except Exception as e:
                if self.running:  # Выводим ошибку только если не остановлены
                    print(f"Ошибка чтения: {e}")
                break

    def send_command(self, command):
        """Отправка команды в Arduino"""
        if not self.ser or not self.ser.is_open:
            print("Не подключено к Arduino!")
            return False

        try:
            self.ser.write((command + '\n').encode('utf-8'))
            print(f"Отправлено: {command}")
            return True
        except Exception as e:
            print(f"Ошибка отправки команды: {e}")
            return False

    def interactive_mode(self):
        """Интерактивный режим с командами"""
        if not self.connect():
            return

        self.start_reading()

        print("\n" + "=" * 50)
        print("Arduino Serial Controller")
        print("=" * 50)
        print("Доступные команды:")
        print("  help - показать это сообщение")
        print("  exit или quit - выход")
        print("  clear - очистить экран")
        print("  status - статус подключения")
        print("  reconnect - переподключиться")
        print("  Любая другая команда будет отправлена в Arduino")
        print("=" * 50)

        while True:
            try:
                command = input(">>> ").strip()

                if command.lower() in ['exit', 'quit']:
                    break
                elif command.lower() == 'help':
                    self.show_help()
                elif command.lower() == 'clear':
                    os.system('cls' if os.name == 'nt' else 'clear')
                elif command.lower() == 'status':
                    self.show_status()
                elif command.lower() == 'reconnect':
                    self.reconnect()
                elif command:
                    self.send_command(command)

            except KeyboardInterrupt:
                print("\nВыход...")
                break
            except Exception as e:
                print(f"Ошибка: {e}")

        self.disconnect()

    def show_help(self):
        """Показать справку по командам"""
        help_text = """
Команды управления:
  help          - показать эту справку
  exit/quit     - выход из программы
  clear         - очистить экран
  status        - показать статус подключения
  reconnect     - переподключиться к Arduino

Команды для Arduino (примеры):
  help          - справка Arduino
  mode_auto     - автоматический режим
  mode_pot      - режим потенциометра
  measure 0 90 10 - автоизмерения от 0° до 90° с шагом 10°
  start-measure - начать непрерывные измерения
  stop          - остановить измерения
  tare          - обнулить весы
  status        - статус системы
  home          - калибровка мотора
  angle         - текущий угол
  motor_on      - включить мотор
  motor_off     - выключить мотор
  50            - переместить на 50 шагов
  -50           - переместить на -50 шагов
        """
        print(help_text)

    def show_status(self):
        """Показать статус подключения"""
        if self.ser and self.ser.is_open:
            print(f"Подключено к: {self.port}")
            print(f"Baudrate: {self.baudrate}")
        else:
            print("Не подключено")

    def reconnect(self):
        """Переподключиться"""
        self.disconnect()
        time.sleep(1)
        self.connect()
        if self.ser and self.ser.is_open:
            self.start_reading()

    def disconnect(self):
        """Отключиться от Arduino"""
        self.running = False
        if self.read_thread:
            self.read_thread.join(timeout=1)
        if self.ser and self.ser.is_open:
            self.ser.close()
        print("Отключено от Arduino")


def main():
    # Автоматическое определение порта или ручной ввод
    controller = ArduinoSerialController(baudrate=9600)

    # Показать доступные порты
    available_ports = controller.list_ports()

    if available_ports:
        port_choice = input("Введите номер порта (Enter для авто выбора): ").strip()
        if port_choice and port_choice.isdigit():
            choice_index = int(port_choice) - 1
            if 0 <= choice_index < len(available_ports):
                selected_port = available_ports[choice_index]
            else:
                print("Неверный выбор, используется первый порт")
                selected_port = available_ports[0]
        else:
            selected_port = available_ports[0]

        controller.port = selected_port
        controller.interactive_mode()
    else:
        print("Не найдено COM портов!")


if __name__ == "__main__":
    main()