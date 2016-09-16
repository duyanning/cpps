#ifndef MINILOGGER_H
#define MINILOGGER_H

//#include <iosfwd>

class MiniLogger {
public:
    MiniLogger(std::ostream& os = std::clog, bool enabled = true, const MiniLogger* super = 0);
    template <class T> MiniLogger& operator<<(const T& x);
    void end_line();
    bool is_enabled() const
    {
        return not disable_all_loggers
            and m_is_enabled
            and (m_super ? m_super->is_enabled() : true);
    }
    void enable() { m_is_enabled = true; }
    void disable() { m_is_enabled = false; }
    operator bool() const { return true; }
    std::ostream& get_os() { return m_os; }
private:
    std::ostream& m_os;
    bool m_is_enabled;
    const MiniLogger* m_super;

public:
    static MiniLogger default_logger;
    static bool disable_all_loggers;
};

inline
MiniLogger::MiniLogger(std::ostream& os, bool enabled, const MiniLogger* super)
           :
    m_os(os),
    m_is_enabled(enabled),
    m_super(super)

{
}


template <typename T>
inline
MiniLogger&
MiniLogger::operator<<(const T& x)
{
    using namespace std;

    if (is_enabled()) {
        m_os << x;
    }

    return *this;
}

inline
void
MiniLogger::end_line()
{
    using namespace std;
    if (is_enabled()) {
        m_os << endl;
    }
}


#define MINILOG_IF(cond, logger, message) {     \
        if (cond) {                             \
            logger << message;                  \
            logger.end_line();                  \
        }                                       \
    }


#define MINILOG(logger, message) MINILOG_IF(true, logger, message)


#define _MINILOG_IF(cond, logger, message) {    \
        if (cond) {                             \
            logger << message;                  \
        }                                       \
    }


#define _MINILOG(logger, message) _MINILOG_IF(true, logger, message)


#define MINILOG0_IF(cond, message) {                            \
        MINILOG_IF(cond, MiniLogger::default_logger, message);  \
    }


#define MINILOG0(message) MINILOG0_IF(true, message)


#define _MINILOG0_IF(cond, message) {                           \
        _MINILOG_IF(cond, MiniLogger::default_logger, message); \
    }


#define _MINILOG0(message) _MINILOG0_IF(true, message)


#define MINILOGPROC_IF(cond, logger, proc, args) {  \
        if (cond) {                                 \
            if (logger.is_enabled()) {              \
                std::ostream& os(logger.get_os());  \
                proc args;                          \
            }                                       \
        }                                           \
    }


#define MINILOGPROC(logger, proc, args) MINILOGPROC_IF(true, logger, proc, args)


#define MINILOGBLK_IF(cond, logger, code)       \
    if (cond) {                                 \
        if (logger.is_enabled()) {              \
            std::ostream& os(logger.get_os());  \
            code                                \
        }                                       \
    }


#define MINILOGBLK(logger, code) MINILOGBLK_IF(true, logger, code)


#endif // MINILOGGER_H
