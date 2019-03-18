#ifndef FILESIG_H
#define FILESIG_H

// 特征相同而文件不同的概率极低
// struct FileSig {
//     fs::path path;              // 文件的路径（这个字段其实是非必须的）
//     time_t timestamp {0};       // 文件的日期
//     uintmax_t size {0};         // 文件的大小

//     friend class boost::serialization::access;
//     template<class Archive>
//     void serialize(Archive & ar, const unsigned int version)
//     {
//         ar & timestamp;
//         ar & size;
//     }
// };

struct FileSig {
    fs::path path;              // 文件的路径（这个字段其实是非必须的）
    string sha1;
    
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & sha1;
    }
};


inline
bool operator==(const FileSig& a, const FileSig& b)
{
    if (a.sha1 == b.sha1)
        return true;
    // if (a.timestamp == b.timestamp && a.size == b.size)
    //     return true;
    return false;
}

inline
bool operator!=(const FileSig& a, const FileSig& b)
{
    return !(a == b);
}

// inline
// ostream& operator<<(ostream& os, const FileSig& sig)
// {
//     return os << sig.path << "\n" << sig.timestamp << "\n" << sig.size << "\n";
// }

// inline
// istream& operator>>(istream& is, FileSig& sig)
// {
//     return is >> sig.path >> sig.timestamp >> sig.size;
// }

#endif // FILESIG_H
