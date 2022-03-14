
v. 0.0.10 -
   ) Для смонтированных файловых систем теперь показывается ёмкость ФС вместо
     ёмкости носителя.  Ёмкость носителя для этих ФС видна, если установлена
     опция "Показывать подробнее".
   ) Добавлен показ меток разделов APT(Mac) и GPT.  Улучшен показ типов
     разделов GPT.

v. 0.0.9 - 2019.06.13
  1) Для действий "Отключить" и "Извлечь" добавлена проверка на loopback-
     отображения.
  2) Устройства показываются в стиле lsblk.  Добавлена опция "Показывать
     подробнее".
  3) Существенно переработаны и унифицированы примеры скриптов, исправлены
     некоторые ошибки, добавлена поддержка yad.
  4) Добавлена интернационализация всех скриптов (в настоящее время доступен
     только русский перевод).
  5) Упрощена установка.  Изменены ".pro" и ".ebuild" файлы.
  6) Улучшена обработка таймаутов при исполнении внешних программ.
  7) Небольшой рефакторинг.
  8) Обновлена документация в соответствии с пп. 1-6.

v. 0.0.8 - 2018.09.13
  1) Улучшена синхронизация конфигурационного файла.
  2) Изменён способ показа сообщения "Устройства не найдены".
  3) Изменение полей "Скрытые устройства" и "Показывать всегда" больше не
     требует перезапуска программы.
  4) Скрипты lock-cryptsetup-* пытаются отмонтировать все точки монтирования
     ФС контейнера, если необходимо.  Скрипты unlock-cryptsetup-* вызывают
     команду монтирования, если в контейнере найдена ФС.  Во всех скриптах
     исправлена обработка имён файлов и точек монтирования, содержащих
     не-ascii и непечатаемые символы.
  5) Улучшен порядок вывода элементов меню.
  6) Изменено поведение действий "Извлечь" и "Отключить".  Добавлено действие
     "Отмонтировать все точки монтирования".
  7) Обновлена документация.

v. 0.0.7 - 2018.06.13
  1) Добавлена возможность "Скрыть иконку/Показать в лотке".
  2) Примеры скриптов существенно пересмотрены.  Добавлены скрипты на Expect и
     Expect+Tk, а также примеры употребления gtkdialog и Xdialog вместо
     qarma/zenity.
  3) Добавлена возможность "Показывать всегда".  Изменён диалог "Настройки".
  4) Изменён формат поля "Скрытые устройства" диалога "Настройки".
  5) Упрощена установка.  Изменены ".pro" и ".ebuild" файлы.
  6) Добавлена и обновлена документация в соответствии с пп. 1-5.
  7) Рефакторинг, небольшие улучшения и исправления некритических ошибок.

v. 0.0.6 - 2017.10.13
  1) Добавлена обработка UNIX-сигналов и корректное завершение по сигналам
     TERM, HUP, INT, QUIT.
  2) Добавлено управление показом меню сигналами SIGUSR1 и SIGUSR2.
  3) Добавлена возможность работы без показа в системном лотке и
     соответствующий параметр командной строки.
  4) Обновлена документация в соответствии с пп. 1-3.
  5) Рефакторинг и исправление регрессий.

v. 0.0.5 - 2017.08.13
  1) Добавлено "дополнительное меню" с пунктами "Извлечь" и "Отключить".
  2) Добавлена настройка "Автоизвлечение".
  3) Добавлены возможность показывать вывод команды при её успешном завершении
     и соответствующие настройки для каждой команды и "Авто..." режима.
  4) Добавлены примеры реализации действия "Отключить" через util-linux/eject
     и через запись в '/sys/.../remove'.
  5) Добавлена установка переменных окружения "TMOUNT_*".
  6) Добавлена и обновлена документация в соответствии с пп. 1-5.
  7) Рефакторинг и исправление регрессий.

v. 0.0.4 - 2017.06.13
  1) Добавлена отдельная настройка для действия "Добавить образ".
  2) Добавлены раздельные настройки таймаутов для всех действий.
  3) Добавлены примеры использования cryptsetup с sudo.
  4) Добавлена поддержка контейнеров LUKS в регулярных файлах.
  5) Добавлена поддержка опции cryptsetup --key-file (и опции pmount -p).
  6) Исправлена документация.
  7) Небольшие исправления и рефакторинг.

v. 0.0.3 - 2017.05.13
  1) Добавлена поддержка монтирования файлов-образов ФС.
  2) Добавлен показ размера устройств.
  3) Добавлен шаблон ebuild для пользователей Gentoo.
  4) Обновлена документация в соответствии с пп. 1-3.
  5) Добавлена русская "документация" и исправлен перевод.
  6) Небольшие исправления и рефакторинг.

v. 0.0.2 - 2017.04.13
  1) Добавлена поддержка LUKS-контейнеров.
  2) Добавлена поддержка регулярных выражений в списке скрытых устройств.
  3) Добавлена и обновлена документация в соответствии с пп. 1-2.
  4) Исправлена обработка точек монтирования с кавычками в имени.
  5) Исправлена обработка точек монтирования устройств, монтируемых по
     символьным ссылкам.
  6) Небольшой рефакторинг.

v. 0.0.1 - 2016.04.13
  Первоначальный выпуск.
