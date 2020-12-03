#pragma once
#include <filesystem>
#include <fstream>


template<typename FUNC>
void processPath(const std::filesystem::path& p, FUNC func) {
    if (!std::filesystem::exists(p)) {         //目录不存在直接返回
        return;
    }

    auto begin = std::filesystem::recursive_directory_iterator(p); //获取文件系统迭代器
    auto end = std::filesystem::recursive_directory_iterator();    //end迭代器 
    for (auto it = begin; it != end; it++) {
        const std::string spacer(it.depth() * 2, ' ');  //这个是用来排版的空格
        auto& entry = *it;
        if (std::filesystem::is_regular_file(entry)) {
            //如果是文件
            func(entry);
            //cout << spacer << "File:" << entry;
            //cout << "(" << filesystem::file_size(entry) << " bytes )" << endl;
        }
        else if (std::filesystem::is_directory(entry)) {
            //cout << spacer << "Dir:" << entry << endl;
        }
    }
}

