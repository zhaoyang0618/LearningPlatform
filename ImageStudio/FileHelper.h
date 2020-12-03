#pragma once
#include <filesystem>
#include <fstream>


template<typename FUNC>
void processPath(const std::filesystem::path& p, FUNC func) {
    if (!std::filesystem::exists(p)) {         //Ŀ¼������ֱ�ӷ���
        return;
    }

    auto begin = std::filesystem::recursive_directory_iterator(p); //��ȡ�ļ�ϵͳ������
    auto end = std::filesystem::recursive_directory_iterator();    //end������ 
    for (auto it = begin; it != end; it++) {
        const std::string spacer(it.depth() * 2, ' ');  //����������Ű�Ŀո�
        auto& entry = *it;
        if (std::filesystem::is_regular_file(entry)) {
            //������ļ�
            func(entry);
            //cout << spacer << "File:" << entry;
            //cout << "(" << filesystem::file_size(entry) << " bytes )" << endl;
        }
        else if (std::filesystem::is_directory(entry)) {
            //cout << spacer << "Dir:" << entry << endl;
        }
    }
}

