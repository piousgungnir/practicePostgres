#Запрос нужных для проекта библиотек
set (TARGET_LINK_PAKCKAGES
        Qt5::Core
        Qt5::Network
        Qt5::Widgets}
)

set (USE_MODULES_QT Core Network Widgets)
find_package(Qt5 REQUIRED ${USE_MODULES_QT})

set (QT_INCLUDES ${Qt5Core_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})

