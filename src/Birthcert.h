#ifndef BIRTHCERT_H
#define BIRTHCERT_H

class Birthcert {
public:
    // 添加一个签名
    void addSig(string path, const FileSig& sig);

    // 检查一个签名是否与出生证明中记录的一致。在证明中不存在，也返回true。
    bool verifySig(string path, const FileSig& sig);
private:
    unordered_map<string, FileSig> m_map; // key：路径；value：签名

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & m_map;
    }
};



#endif // BIRTHCERT_H
