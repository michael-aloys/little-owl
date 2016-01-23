TEMPLATE = subdirs

SUBDIRS += \
    Child \
    Parent \
    Common
    
Child.depends = Common
Parent.depends = Common
