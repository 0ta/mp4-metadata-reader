// ConsoleApplication1.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <string>
#include <metadataManager.h>

void printHEXFromCharArray(unsigned char cs[], size_t size);
void printHEXFromChar(unsigned char c);

int main()
{
    std::cout << "Hello World!\n";
    const char* input = "D:\\tmp\\Record_PlaneAndMeshingAR_1201_2132_32.MP4";
    //decode_and_encode_video(output, input);
    uint32_t size = loadMetadata(input);
    std::cout << "size||" << size << "||" << std::endl;
    uint8_t* data = nullptr;
    uint32_t len = peekMetadata(0, data);
    printHEXFromCharArray(data, len);
}

void printHEXFromCharArray(unsigned char cs[], size_t size) {
    for (int i = 0; i < size; ++i) {
        std::cout << "[" << i << "]";
        printHEXFromChar(cs[i]);
    }
}

void printHEXFromChar(unsigned char c) {
    std::printf("0x%02X ", static_cast<unsigned char>(c));
    std::cout << std::endl;
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
