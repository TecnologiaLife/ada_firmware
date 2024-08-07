#ifndef FACE_ANIMATION_HPP
#define FACE_ANIMATION_HPP

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "face_data.hpp"


namespace Face
{
    enum class FacialExpression
    {
        None,
        Normal,
        Angry,
        Sad,
        Happy,
        // ここまで
        ItemIndex
    };


    class FaceAnimation
    {
        private:
            static constexpr uint8_t kInterval = 100;       // [ms] intervalミリ秒毎に実行
            static constexpr uint8_t kScreenWidth = 128;    // [px] 画面幅
            static constexpr uint8_t kScreenHeight = 64;    // [px] 画面高さ
            static constexpr int8_t kOledReset = -1;        // 使用しないので-1を設定
            static constexpr uint8_t kI2CAddress = 0x3C;    // SSD1306 I2Cアドレス
        
            Adafruit_SSD1306* display;      // ディスプレイ制御クラスインスタンスをここで持つ
            uint32_t prev_ms = 0;           // 前回実行時間
            FacialExpression now_fe;        // 現在の表情ステート
            FacialExpression tgt_fe;        // 指示されている表情ステート
            bool f_now_blink = false;       // 瞬きイベント実行中か否か
            uint32_t prev_blink_time = 0;   // 前回イベント時間
            uint8_t blink_frame = 0;        // 瞬きアニメーションの現在フレーム数

            void draw(Face::FacialExpression fe);
            void drawBlink();
            FaceData getFaceData(Face::FacialExpression fe);
            bool blinkJudge(FacialExpression face, uint32_t last_blink_time);

        public:
            FaceAnimation();
            ~FaceAnimation();

            void begin();
            void update();
            void setFacialExpression(FacialExpression fe);
            FacialExpression getFacialExpression();
    };
};



#endif