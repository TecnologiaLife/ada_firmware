#include "FaceAnimation.hpp"




Face::FaceAnimation::FaceAnimation()
{
    this->now_fe = FacialExpression::None;
    this->tgt_fe = FacialExpression::None;

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

    // 描画する表情をセット
    if(this->now_fe != this->tgt_fe && !this->f_now_event)
    {
        this->now_fe = this->tgt_fe;
    }

    // 表情を描画
    this->draw(this->now_fe);

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
    if(f_now_event){ return; }

    FaceData face_frame = getFaceData(fe);

    uint16_t offset_start_address = static_cast<uint8_t>(face_frame) * (kScreenWidth / 8) * kScreenHeight;
    this->display->fillScreen(BLACK);
    this->display->drawBitmap(0, 0, face_data + offset_start_address, 128, 64, WHITE);
    this->display->display();
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