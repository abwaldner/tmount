      Способ разблокирования контейнера LUKS существенно зависит от
  конкретного окружения.  Поэтому по умолчанию конфигурация tmount не
  задаёт никакого действия для таких устройств.  Вы можете выбрать
  подходящие скрипты из имеющихся в каталоге /usr/share/tmount/docs
  (или в подкаталоге ./examples каталога проекта), модифицировать их в
  соответствии с вашими возможностями и предпочтениями, поместить в
  любое доступное для исполнения место и отредактировать поля
  "Разблокировать" и "Блокировать" диалога настроек tmount.
      Команда разблокирования получает параметр, такой же, как команда
  монтирования.
      Команда блокирования получает параметром имя, с которым открывает
  контейнер в каталоге /dev/mapper утилита cryptsetup (без имени
  каталога).
      Команда разблокирования требует интерактивного ввода пароля LUKS
  или имени файла ключа, и обе команды могут требовать интерактивного
  ввода пароля, если реализованы через использование su или sudo.
  Поэтому tmount по умолчанию запускает их без установки таймаута.
  Будьте осторожны: в таком случае зависание этих скриптов блокирует
  работу приложения.  По этой же причине контейнеры никогда не
  разблокируются автоматически.
      Программа pmount может самостоятельно разблокировать и
  автоматически блокировать контейнер LUKS после отмонтирования
  содержащейся в нём файловой системы и не требует явного вызова утилиты
  cryptsetup.
      Пользователи udevil должны добавить, по крайней мере, "/dev/dm*" к
  параметру "allowed_internal_devices" в файле udevil.conf.
      Пользователи sudo могут существенно упростить скрипты,
  использующие утилиту cryptsetup, если получат право на её выполнение
  без ввода пароля.  Кроме того, при использовании эмуляторов терминала
  или утилит наподобие ktsuss теряются код возврата и содержимое stderr
  вызываемой программы.
      Для разблокирования контейнеров, находящихся в регулярных файлах,
  следует обеспечить выполнение соответствующих скриптов через поле
  "Открыть образ" диалога настроек (см. img-*.sh в примерах).
  Предупреждение: не забудтье изменить значение таймаута.
  Замечание: программа pmount работает в данном случае некорректно,
  поэтому необходимо предварительное разблокирование контейнера и
  добавление "/dev/dm-*" в файл /etc/pmount.allow.
      Замечания по реализации:
      Использование в примерах программы qarma
  (http://github.com/luebking/qarma) и собственной обёртки для задания
  переменной SUDO_ASKPASS вызвано тем, что GTK-программы выводят много
  мусора в stderr.  Поэтому использование таких программ как
  ssh-askpass-fullscreen, x11-ssh-askpass, gtkdialog, xdialog (и даже
  zenity) и пр. возможно, но значительно менее удобно.