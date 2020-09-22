#include "ReadBarcode.h"
#include "GenericLuminanceSource.h"
#include "HybridBinarizer.h"
#include "qrcode/QRReader.h"

#include <string>
#include <memory>
#include <stdexcept>

extern "C" {
    const wchar_t* ZXing_ReadBarcode(uint8_t *ptr, int width, int height) {
        using namespace ZXing;
        static std::wstring text;
        try {
            const auto format = ImageFormat::RGBX;
            const auto pixStride = PixStride(format);
            const auto rowStride = width * pixStride;
            GenericLuminanceSource src {
                0, 0, width, height, ptr, rowStride, pixStride,
                RedIndex(format), GreenIndex(format), BlueIndex(format), nullptr};
            DecodeHints hints;
            bool tryHarder = true;
            hints.setTryHarder(tryHarder);
            hints.setTryRotate(tryHarder);

            QRCode::Reader reader(hints);
            auto srcPtr = std::shared_ptr<LuminanceSource>(&src, [](void*) {});
            auto result = reader.decode(HybridBinarizer(srcPtr));
            if (result.isValid()) {
                text.assign(result.text());
                return text.c_str();
            }
        } catch (...) {}
        return nullptr;
    }
}
