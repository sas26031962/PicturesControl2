#ifndef CENVIRONMENT_H
#define CENVIRONMENT_H

/******************************************************************************
 * === Выбор вариантов работы осуществляется автоматически
 * по типу операционной системы
 * ===
 *****************************************************************************/

#define WINDOWS_SYSTEM_TYPE 1
#define LINUX_SYSTEM_TYPE 2

class cEnvironment
{
public:
    static int iSystemType;


    cEnvironment();
};

#endif // CENVIRONMENT_H
