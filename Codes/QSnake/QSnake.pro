HEADERS += \
    configuration.h \
    direction.h \
    game.h \
    game_double.h \
    game_over_window.h \
    game_over_window_double.h \
    game_over_window_single.h \
    game_single.h \
    game_window.h \
    game_window_double.h \
    game_window_single.h \
    pause_window.h \
    setting_window.h \
    snake.h \
    start_window.h

SOURCES += \
    configuration.cpp \
    direction.cpp \
    game.cpp \
    game_double.cpp \
    game_over_window.cpp \
    game_over_window_double.cpp \
    game_over_window_single.cpp \
    game_single.cpp \
    game_window.cpp \
    game_window_double.cpp \
    game_window_single.cpp \
    main.cpp \
    pause_window.cpp \
    setting_window.cpp \
    snake.cpp \
    start_window.cpp

QT += widgets

FORMS += \
    game_over_window_double.ui \
    game_over_window_single.ui \
    game_window_double.ui \
    game_window_single.ui \
    pause_window.ui \
    setting_window.ui \
    start_window.ui

RESOURCES += \
    resources.qrc
