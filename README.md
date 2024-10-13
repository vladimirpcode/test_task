# test_task

## Замечания по выполнению

1. Не стал оформлять в виде службы systemd, т.к. на моей Fedora 40 работе службы мешал SELinux и другие неведомые причины. Сделал в виде фонового процесса, который запускается из консоли.
2. Для работы на Ubuntu пришлось вводить команду sudo sysctl -w net.ipv4.ping_group_range="0 1000", чтобы разрешить создавать ICMP сокет (0, 1000 - id груп рута и юзера на моей машине)
3. Зависимости стандартные: compiler & std c, c++, make. На Fedora для сборки .deb пакета нужен dpkg-dev.
4. Файл данных test-pinger-data.txt создается в домашней директории. Минус в том, что при удалении пакета с ключем purge удалится лишь файл данных у рута.
5. Информация о пингах сохраняется только по прошествии всех пинг-запросов. Т.е. если отправить 100 запросов, то статистика сохранится не ранее чем через 100 секунд.

| | Основной функционал | deb-пакет |
|-|---------------------|-----------|
|Fedora 40| ✅ | ❌ |
|Debian 12| ✅ | ✅ |
|Ubuntu 24| ✅ | ✅ |

## Запуск с deb пакетом

```
git clone https://github.com/vladimirpcode/test_task
cd test_task
make deb
sudo apt install ./*.deb -y
```
После установки запускается командой:
```
test-pinger
```
## Запуск без deb пакета
```
git clone https://github.com/vladimirpcode/test_task
cd test_task
make test-pinger
```
запуск из папки с проектом
```
./test-pinger
```
## Использование
После запуска процесс переходит в фоновый режим и слушает порт 65236.

Подключаемся по telnet:
```
telnet 127.0.0.1 65236
```
Список команд:
```
ping <ip> <количество пингов>
info
```
