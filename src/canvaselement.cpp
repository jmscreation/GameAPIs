#include "canvaselement.h"

namespace olc {

    CanvasElement::CanvasElement(uint32_t width, uint32_t height) {
        Create(width, height);
		Clear(olc::BLANK);
    }

    CanvasElement::~CanvasElement() {

    }


    void CanvasElement::Draw(View* view) {
        if(view != nullptr) {
            view->DrawRotatedDecal(position, Decal(), rotation, origin, scale, blend);
        } else {
            pge->DrawRotatedDecal(position, Decal(), rotation, origin, scale, blend);
        }
    }

	/*
		Canvas Draw Functions
	*/

	void CanvasElement::Clear(Pixel p) {
		olc::Sprite* lastTarget = pge->GetDrawTarget();
        pge->SetDrawTarget(this->Sprite());
        pge->Clear(p);
        pge->SetDrawTarget(lastTarget);
        this->Decal()->Update();
	}

    bool CanvasElement::DrawPixel(int32_t x, int32_t y, olc::Pixel p)
	{
		return DrawPixel({ x, y }, p);
	}

	bool CanvasElement::DrawPixel(const olc::vi2d& pos, olc::Pixel p)
	{
        olc::Sprite* lastTarget = pge->GetDrawTarget();
        pge->SetDrawTarget(this->Sprite());
        bool r = pge->Draw(pos, p);
        pge->SetDrawTarget(lastTarget);
        this->Decal()->Update();

        return r;
	}

	void CanvasElement::DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, olc::Pixel p, uint32_t pattern)
	{
		DrawLine({ x1, y1 }, { x2, y2 }, p, pattern);
	}

	void CanvasElement::DrawLine(const olc::vi2d& pos1, const olc::vi2d& pos2, olc::Pixel p, uint32_t pattern)
	{
		olc::Sprite* lastTarget = pge->GetDrawTarget();
        pge->SetDrawTarget(this->Sprite());
        pge->DrawLine(pos1, pos2, p, pattern);
        pge->SetDrawTarget(lastTarget);
        this->Decal()->Update();
	}

	void CanvasElement::DrawCircle(int32_t x, int32_t y, int32_t radius, olc::Pixel p, uint8_t mask)
	{
		DrawCircle({ x,y }, radius, p, mask);
	}

	void CanvasElement::DrawCircle(const olc::vi2d& pos, int32_t radius, olc::Pixel p, uint8_t mask)
	{
		olc::Sprite* lastTarget = pge->GetDrawTarget();
        pge->SetDrawTarget(this->Sprite());
        pge->DrawCircle(pos, int32_t(radius), p, mask);
        pge->SetDrawTarget(lastTarget);
        this->Decal()->Update();
	}

	void CanvasElement::FillCircle(int32_t x, int32_t y, int32_t radius, olc::Pixel p)
	{
		FillCircle({ x,y }, radius, p);
	}

	void CanvasElement::FillCircle(const olc::vi2d& pos, int32_t radius, olc::Pixel p)
	{
		olc::Sprite* lastTarget = pge->GetDrawTarget();
        pge->SetDrawTarget(this->Sprite());
        pge->FillCircle(pos, int32_t(radius), p);
        pge->SetDrawTarget(lastTarget);
        this->Decal()->Update();
	}

	void CanvasElement::DrawRect(int32_t x, int32_t y, int32_t w, int32_t h, olc::Pixel p)
	{
		DrawRect({ x, y }, { w, h }, p);
	}

	void CanvasElement::DrawRect(const olc::vi2d& pos, const olc::vi2d& size, olc::Pixel p)
	{
		olc::Sprite* lastTarget = pge->GetDrawTarget();
        pge->SetDrawTarget(this->Sprite());
        pge->DrawRect(pos, size, p);
        pge->SetDrawTarget(lastTarget);
        this->Decal()->Update();
	}

	void CanvasElement::FillRect(int32_t x, int32_t y, int32_t w, int32_t h, olc::Pixel p)
	{
		FillRect({ x, y }, { w, h }, p);
	}

	void CanvasElement::FillRect(const olc::vi2d& pos, const olc::vi2d& size, olc::Pixel p)
	{
		olc::Sprite* lastTarget = pge->GetDrawTarget();
        pge->SetDrawTarget(this->Sprite());
        pge->FillRect(pos, size, p);
        pge->SetDrawTarget(lastTarget);
        this->Decal()->Update();
	}

	void CanvasElement::DrawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, olc::Pixel p)
	{
		DrawTriangle({ x1, y1 }, { x2, y2 }, { x3, y3 }, p);
	}

	void CanvasElement::DrawTriangle(const olc::vi2d& pos1, const olc::vi2d& pos2, const olc::vi2d& pos3, olc::Pixel p)
	{
		olc::Sprite* lastTarget = pge->GetDrawTarget();
        pge->SetDrawTarget(this->Sprite());
        pge->DrawTriangle(pos1, pos2, pos3, p);
        pge->SetDrawTarget(lastTarget);
        this->Decal()->Update();
	}

	void CanvasElement::FillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, olc::Pixel p)
	{
		FillTriangle({ x1, y1 }, { x2, y2 }, { x3, y3 }, p);
	}

	void CanvasElement::FillTriangle(const olc::vi2d& pos1, const olc::vi2d& pos2, const olc::vi2d& pos3, olc::Pixel p)
	{
		olc::Sprite* lastTarget = pge->GetDrawTarget();
        pge->SetDrawTarget(this->Sprite());
        pge->FillTriangle(pos1, pos2, pos3, p);
        pge->SetDrawTarget(lastTarget);
        this->Decal()->Update();
	}

	void CanvasElement::DrawSprite(int32_t x, int32_t y, olc::Sprite* sprite, uint32_t scale, uint8_t flip)
	{
		DrawSprite({ x, y }, sprite, scale, flip);
	}

	void CanvasElement::DrawSprite(const olc::vi2d& pos, olc::Sprite* sprite, uint32_t scale, uint8_t flip)
	{
		olc::Sprite* lastTarget = pge->GetDrawTarget();
        pge->SetDrawTarget(this->Sprite());
        pge->DrawSprite(pos, sprite, scale, flip);
        pge->SetDrawTarget(lastTarget);
        this->Decal()->Update();
	}

	void CanvasElement::DrawPartialSprite(int32_t x, int32_t y, olc::Sprite* sprite, int32_t ox, int32_t oy, int32_t w, int32_t h, uint32_t scale, uint8_t flip)
	{
		DrawPartialSprite({ x,y }, sprite, { ox,oy }, { w, h }, scale, flip);
	}

	void CanvasElement::DrawPartialSprite(const olc::vi2d& pos, olc::Sprite* sprite, const olc::vi2d& sourcepos, const olc::vi2d& size, uint32_t scale, uint8_t flip)
	{
		olc::Sprite* lastTarget = pge->GetDrawTarget();
        pge->SetDrawTarget(this->Sprite());
        pge->DrawPartialSprite(pos, sprite, sourcepos, size, scale, flip);
        pge->SetDrawTarget(lastTarget);
        this->Decal()->Update();
	}

	void CanvasElement::DrawString(int32_t x, int32_t y, const std::string& sText, Pixel col, uint32_t scale)
	{
		DrawString({ x, y }, sText, col, scale);
	}

	void CanvasElement::DrawString(const olc::vi2d& pos, const std::string& sText, const Pixel col, uint32_t scale)
	{
        olc::Sprite* lastTarget = pge->GetDrawTarget();
        pge->SetDrawTarget(this->Sprite());
        pge->DrawString(pos, sText, col, scale);
        pge->SetDrawTarget(lastTarget);
        this->Decal()->Update();
	}
}