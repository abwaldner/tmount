
      Перед запуском команды "Отключить" приложение проверяет, не используется
  ли указанное устройство.  Заданная по умолчанию команда не делает ничего, и
  вызывается только для осуществления этой проверки и показа её результата.
  Этого почти всегда достаточно (см., тем не менее, замечание в README.ru).
      Однако вы можете настроить "отключение" через использование программы
  eject из пакета util-linux.  Возьмите скрипт remove1.sh из каталога
  /usr/share/tmount/examples/, поместите его в любое доступное для исполнения
  место и отредактируйте поле "Отключить" в диалоге настроек приложения.
  Обеспечьте права на запись к файлу устройства.  Вы можете, например,
  поместить файл 65-plugdev.rules из того же каталога в каталог
  /etc/udev/rules.d/ для изменения группы файла устройства на "plugdev".
  Пользователи udevil и pmount уже должны состоять в этой группе.  ("65"
  обеспечивает выполнение правил после набора "60-persistent-storage.rules").
  Или пользователи sudo, имеющие право на исполнение команды eject без ввода
  пароля, могут отредактировть скрипт, заменив вызов "eject..." на
  "sudo -n eject...".  Или вы можете воспользоваться любым из способов
  исполнения команды через "su" или "sudo", продемонстрированными в примерах в
  вышеупомянутом каталоге.
      Применение eject к устройствам, не имеющим атрибута "removable",
  например, к внешним USB SDD, заканчивается успешно, но не оказывает
  необходимого действия.  Устройство просто переподключается, что при
  включённом режиме автомонтирования или работающем стороннем автомонтировщике
  создаёт проблемы.  Поэтому скрипт просто отказывется это делать.
      Вы можете отключать такие устройства путём записи в соответствующий файл
  sysfs, как это делает программа udisk (см. remove2.sh), однако делать это
  весьма не рекомендуется.
