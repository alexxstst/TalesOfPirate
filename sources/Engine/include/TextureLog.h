#pragma once

//  Лог загрузки/выгрузки текстур в видеопамяти. Раньше был встроен в
//  lwResourceMgr (lwTexLogMgr/lwTexLogFilterInfo) и писал в собственный
//  файл .\log\game\tex.log через FILE*+sprintf. Теперь — синглтон с
//  выходом через ToLogService("textures", ...) и runtime-тоглом из INI.

#include "lwDirectX.h"

#include <cstdint>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>


namespace Corsairs::Engine::Render {

    //  Тип события, передаваемый в TextureLog::Log.
    enum class TextureLogOp : std::uint32_t
    {
        //  Текстура залита в VRAM — увеличиваем счётчики.
        LOAD = 0,

        //  Текстура освобождена — уменьшаем счётчики.
        RELEASE = 1,
    };


    //  Учёт VRAM-расхода текстур и вывод событий в канал "textures"
    //  через ToLogService. Категории — substring-фильтры по имени файла,
    //  накапливают пер-категорийные count/bytes (сравнение substring'ом
    //  даёт false-positive для коротких ключей вроде "ui" — это сознательный
    //  компромисс, исторически унаследованный от lwTexLogMgr).
    //  По умолчанию выключен. Включается из клиента после GlobalAppConfig.Load()
    //  если в [TextureLog] enabled = 1. Когда выключен — Log() это no-op,
    //  так что 5 колсайтов в lwResourceMgr работают и в проде без накладных.
    class TextureLog
    {
    public:
        static TextureLog& Instance();

        TextureLog(const TextureLog&)            = delete;
        TextureLog& operator=(const TextureLog&) = delete;

        //  Toggle. Когда выключаем после работы — пишем итоговую сводку.
        void SetEnabled(bool enabled);
        bool IsEnabled() const;

        //  Регистрировать стоит до начала загрузок, чтобы пер-категорийные
        //  счётчики начали накапливаться с первого события.
        void RegisterCategory(std::string_view substring);

        //  Записать событие. При отключённом логе — мгновенный возврат.
        void Log(TextureLogOp op,
                 std::string_view file,
                 std::uint32_t width,
                 std::uint32_t height,
                 D3DFORMAT format,
                 std::uint32_t devmemSize);

    private:
        struct Category
        {
            std::string Substring;
            std::uint64_t TotalBytes{0};
            std::uint64_t Count{0};
        };

        TextureLog();
        ~TextureLog();

        //  Печать сводки по всем категориям и тоталам в канал "textures".
        //  Вызывается при выключении и в дтрукторе (если что-то накопили).
        void EmitSummary();

        std::mutex _mutex;
        std::vector<Category> _categories;
        std::uint64_t _totalBytes{0};
        std::uint64_t _totalCount{0};
        bool _enabled{false};
    };

} // namespace Corsairs::Engine::Render
