#include "FaceAnimation.hpp"




Face::FaceAnimation::FaceAnimation()
{
    this->now_fe = FacialExpression::None;
    this->tgt_fe = FacialExpression::None;
    this->prev_blink_time = millis();

    // ディスプレイ制御クラスインスタンス生成
    this->display = new Adafruit_SSD1306(kScreenWidth, kScreenHeight, &Wire, kOledReset);
}




Face::FaceAnimation::~FaceAnimation()
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"

    // ディスプレイ制御クラスインスタンス消去

    /**
     * Adafruit_SSD1306のクラスdeleteについて未定義動作の警告が出るが、
     * 基本的にFaceAnimationクラスのデストラクタは呼ばれない為無視する。
     * FaceAnimationクラスのデストラクタが呼ばれない理由：呼ばれる前にマイコン電源が落ちる。
     * 
     * 「警告文」
     * deleting object of polymorphic class type 'Adafruit_SSD1306' 
     * which has non-virtual destructor might cause undefined behavior [-Wdelete-non-virtual-dtor]
     */
    delete this->display;

#pragma GCC diagnostic pop
}




/**
 * @fn
 * @brief ディスプレイとの通信を開始します
 */
void Face::FaceAnimation::begin()
{
    if(!this->display->begin(SSD1306_SWITCHCAPVCC, kI2CAddress))
    {
      for(;;);  // [TODO] 無限ループ危険なのでエラーハンドラに落としてもいいかも
    }

    this->display->clearDisplay(); //何か表示されている場合に備えて表示クリア
}




/**
 * @fn
 * @brief 表示させたい表情をセットします
 * @param fe 表情です
 */
void Face::FaceAnimation::setFacialExpression(FacialExpression fe)
{
    if(!(fe < FacialExpression::ItemIndex)){ return; }
    this->tgt_fe = fe;
}




/**
 * @fn
 * @brief アップデート関数
 * @details 定期的に呼び出す必要があります
 */
void Face::FaceAnimation::update()
{
    uint32_t now_ms = millis();
    if((now_ms - prev_ms) < kInterval){ return; }

    // 瞬きするか判断する
    if(!this->f_now_blink){ this->f_now_blink = blinkJudge(this->now_fe, this->prev_blink_time); }

    if(this->f_now_blink)
    {
        // 瞬き描画
        this->drawBlink();
    }
    else
    {
        // 指示で受け取った表情が現在の表情と異なる場合、表情を更新する
        this->now_fe = (this->now_fe != this->tgt_fe) ? this->tgt_fe : this->now_fe;
        // 表情を描画する
        this->draw(this->now_fe);
    }

    // 時刻の更新
    prev_ms = now_ms;
}




/**
 * @fn
 * @brief 現在表示されている表情を取得します
 * @return 現在表示されている表情
 */
Face::FacialExpression Face::FaceAnimation::getFacialExpression()
{
    return this->now_fe;
}








/* ===== PRIVATE METHODS ===== */

/**
 * @fn
 * @brief 表情を描画します
 */
void Face::FaceAnimation::draw(Face::FacialExpression fe)
{
    if(f_now_blink){ return; }

    FaceData face_frame = getFaceData(fe);

    uint16_t offset_start_address = static_cast<uint8_t>(face_frame) * (kScreenWidth / 8) * kScreenHeight;
    this->display->fillScreen(BLACK);
    this->display->drawBitmap(0, 0, face_data + offset_start_address, 128, 64, WHITE);
    this->display->display();
}




/**
 * @fn
 * @brief 瞬きを描画します
 */
void Face::FaceAnimation::drawBlink()
{
    uint16_t offset_start_address = 0;

    switch(this->blink_frame)
    {
        case 0:
            offset_start_address = static_cast<uint8_t>(FaceData::Normal_01) * (kScreenWidth / 8) * kScreenHeight;
            break;
        case 1:
            offset_start_address = static_cast<uint8_t>(FaceData::Normal_02) * (kScreenWidth / 8) * kScreenHeight;
            break;
        case 2:
            offset_start_address = static_cast<uint8_t>(FaceData::Normal_03) * (kScreenWidth / 8) * kScreenHeight;
            break;
        case 3:
            offset_start_address = static_cast<uint8_t>(FaceData::Normal_04) * (kScreenWidth / 8) * kScreenHeight;
            break;
        case 4:
            offset_start_address = static_cast<uint8_t>(FaceData::Normal_05) * (kScreenWidth / 8) * kScreenHeight;
            break;
        case 5:
            offset_start_address = static_cast<uint8_t>(FaceData::Normal_04) * (kScreenWidth / 8) * kScreenHeight;
            break;
        case 6:
            offset_start_address = static_cast<uint8_t>(FaceData::Normal_03) * (kScreenWidth / 8) * kScreenHeight;
            break;
        case 7:
            offset_start_address = static_cast<uint8_t>(FaceData::Normal_02) * (kScreenWidth / 8) * kScreenHeight;
            break;
        case 8:
            offset_start_address = static_cast<uint8_t>(FaceData::Normal_01) * (kScreenWidth / 8) * kScreenHeight;
            break;
        default:
            break;
    }

    this->display->fillScreen(BLACK);
    this->display->drawBitmap(0, 0, face_data + offset_start_address, 128, 64, WHITE);
    this->display->display();

    if(this->blink_frame == 8)
    {
        // クリア
        this->f_now_blink = false;
        this->blink_frame = 0;
        // 瞬きした時刻を更新
        this->prev_blink_time = millis();
    }
    else
    {
        // フレームをインクリメント
        this->blink_frame++;
    }
}




/**
 * @fn
 * @brief 表情から該当するFaceDataを取得します
 * @param fe 描画したい表情
 * @return 描画するFaceData
 */
FaceData Face::FaceAnimation::getFaceData(Face::FacialExpression fe)
{
    FaceData fd;

    switch(fe)
    {
        case Face::FacialExpression::Normal:
            fd = FaceData::Normal_01;
            break;

        case Face::FacialExpression::Angry:
            fd = FaceData::Angry_01;
            break;

        case Face::FacialExpression::Sad:
            fd = FaceData::Sad_01;
            break;

        case Face::FacialExpression::Happy:
            fd = FaceData::Happy_01;
            break;

        default:
            fd = FaceData::Normal_01;
            break;
    }

    return fd;
}




/**
 * @fn
 * @brief 瞬きするかどうかを判断します
 * @details 最後に瞬きした時間から3秒経過後、5%の確率でtrue。最後に瞬きした時間から3秒以内、2%の確率でtrue
 * @param last_blink_time 最後に瞬きした時間
 * @return true or false
 */
bool Face::FaceAnimation::blinkJudge(FacialExpression face, uint32_t last_blink_time)
{
    constexpr uint16_t kDiffTimeThres = 3000;   // 前回瞬き時間からの閾値
    bool ret = false;                           // 返り値変数定義
    uint32_t diff = millis() - last_blink_time; // 前回瞬きからの経過時間を計算
    uint16_t random_num = random(1, 1000);       // 1 ~ 100の値の乱数を生成

    if(face == FacialExpression::Normal)
    {
        if(diff > kDiffTimeThres){ ret = (random_num > 950) ? true : false; }
        else                     { ret = (random_num > 980) ? true : false; }
    }
    else
    {
        ret = false;
    }

    return ret;
}