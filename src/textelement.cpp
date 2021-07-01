#include "textelement.h"


namespace olc {
    
    TextElement::TextElement(olc::FontAsset fnt, const std::string& string):
            font(fnt), textData(string), texture(nullptr), color(olc::WHITE),
            size({0, 0}), hTextAlign(Align::NONE), vTextAlign(Align::NONE) {
        UpdateTexture();
    }

    TextElement::TextElement(olc::FontAsset fnt):
            font(fnt), textData(""), texture(nullptr), color(olc::WHITE),
            size({0, 0}), hTextAlign(Align::NONE), vTextAlign(Align::NONE) {
        UpdateTexture();
    }

    TextElement::~TextElement() {
        delete texture;
    }

    void TextElement::UpdateTexture() {
        size = font->GetStringBounds(textData).size;
        size = size.max(olc::vi2d({16,16}));

        switch(hTextAlign){ // update x origin based on auto text alignment
            case Align::NONE: break;
            case Align::LEFT: origin.x = 0; break;
            case Align::RIGHT: origin.x = size.x; break;
        }

        switch(vTextAlign){ // update y origin based on auto text alignment
            case Align::NONE: break;
            case Align::TOP: origin.y = 0; break;
            case Align::BOTTOM: origin.y = size.y; break;
        }

        if(texture == nullptr){
            auto spr = std::make_shared<olc::Sprite>(size.x, size.y);
            texture = new ResourceTexture( { spr, olc::Decal(spr.get()) });
        } else {
            olc::Sprite& spr = *(texture->sprite);
            if(spr.width < size.x || spr.height < size.y){ // Manually Resize Sprite -- oof
                spr.width = size.x;
                spr.height = size.y;
                spr.pColData.resize(size.x * size.y, olc::BLANK);
            }
        }

        if(font->RenderStringToSprite(textData, color, texture->sprite.get()) == nullptr) return;

        texture->texture.Update();
    }

    void TextElement::UpdateString(const std::string& string) {
        textData = string;

        UpdateTexture();
    }

    void TextElement::Draw(View* view) {
        if(view != nullptr) {
            view->DrawRotatedDecal(position, &texture->texture, rotation, origin, scale, blend);
        } else {
            pge->DrawRotatedDecal(position, &texture->texture, rotation, origin, scale, blend);
        }
    }
}