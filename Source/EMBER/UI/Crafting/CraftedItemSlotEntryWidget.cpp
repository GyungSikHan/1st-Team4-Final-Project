#include "UI/Crafting/CraftedItemSlotEntryWidget.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/ItemTemplate.h"
#include "UI/Data/EmberUIData.h"

UCraftedItemSlotEntryWidget::UCraftedItemSlotEntryWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer),
    CachedItemTemplateClass(nullptr),
    CachedRarity(EItemRarity::Common),
    CachedQuantity(0),
    SizeBox_Root(nullptr),
    ItemIcon(nullptr),
    QuantityText(nullptr)
{
}

void UCraftedItemSlotEntryWidget::SetSlotData(TSubclassOf<UItemTemplate> InItemTemplateClass, EItemRarity InRarity, int32 InQuantity)
{
    CachedItemTemplateClass = InItemTemplateClass;
    CachedRarity = InRarity;
    CachedQuantity = InQuantity;

    if (!InItemTemplateClass && InQuantity > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Unexpected: InQuantity > 0인데 템플릿이 NULL임"));
    }
    UE_LOG(LogTemp, Warning, TEXT("[WIDGET] SetSlotData called: Class=%s / Rarity=%d / Qty=%d"),
        InItemTemplateClass ? *InItemTemplateClass->GetName() : TEXT("NULL"),
        static_cast<uint8>(InRarity),
        InQuantity);

    UpdateWidgetAppearance();
}

void UCraftedItemSlotEntryWidget::UpdateWidgetAppearance()
{
    const UItemTemplate* Template = nullptr;

    if (CachedItemTemplateClass)
    {
        Template = CachedItemTemplateClass->GetDefaultObject<UItemTemplate>();
        if (!Template)
        {
            UE_LOG(LogTemp, Warning, TEXT("[UI] Template CDO is NULL for class: %s"), *CachedItemTemplateClass->GetName());
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[UI] CachedItemTemplateClass is NULL"));
    }

    const bool bHasItem = (Template && CachedQuantity > 0);

    // 슬롯 사이즈 설정
    if (SizeBox_Root)
    {
        const FIntPoint UnitSize = UEmberUIData::Get().UnitInventorySlotSize;
        float Width = UnitSize.X;
        float Height = UnitSize.Y;

        if (Template && Template->SlotCount.X > 0 && Template->SlotCount.Y > 0)
        {
            Width *= Template->SlotCount.X;
            Height *= Template->SlotCount.Y;
        }

        SizeBox_Root->SetWidthOverride(90.f);
        SizeBox_Root->SetHeightOverride(180.f);
        UE_LOG(LogTemp, Log, TEXT("[UI] Slot Size Set: %.0fx%.0f | Item: %s"),
            Width, Height,
            Template ? *Template->GetName() : TEXT("NULL"));
    }

    // 아이콘 표시
    if (ItemIcon)
    {
        const bool bValidIcon = (Template && Template->IconTexture);
        if (bValidIcon)
        {
            UTexture2D* Texture = Cast<UTexture2D>(Template->IconTexture);
            if (Texture)
            {
                UE_LOG(LogTemp, Warning, TEXT("[DEBUG] IconTexture IsValid: %s, Name: %s"),
                    Texture ? TEXT("TRUE") : TEXT("FALSE"),
                    Texture ? *Texture->GetName() : TEXT("NULL"));
                // 엔진 내장 방식으로 브러시 설정
                ItemIcon->SetBrushFromTexture(Texture, false);
                ItemIcon->Brush.ImageSize = FVector2D(Texture->GetSizeX(), Texture->GetSizeY());
                ItemIcon->SetRenderTransform(FWidgetTransform()); // 스케일 초기화
                ItemIcon->SetRenderScale(FVector2D(1.f, 1.f));
                ItemIcon->SetVisibility(ESlateVisibility::Visible);
                ItemIcon->InvalidateLayoutAndVolatility();
                UE_LOG(LogTemp, Warning, TEXT("[DEBUG] ItemIcon Visibility: %s"), *UEnum::GetValueAsString(ItemIcon->GetVisibility()));
                UE_LOG(LogTemp, Warning, TEXT("[DEBUG] ItemIcon Render Opacity: %.2f"), ItemIcon->GetRenderOpacity());
                UE_LOG(LogTemp, Warning, TEXT("[DEBUG] ItemIcon Desired Size: %s"), *ItemIcon->GetDesiredSize().ToString());

                UE_LOG(LogTemp, Log, TEXT("[UI] Icon Set by SetBrushFromTexture: %s (%dx%d)"),
                    *Texture->GetName(), Texture->GetSizeX(), Texture->GetSizeY());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("[UI] IconTexture is not a UTexture2D!"));
            }
        }

        // 유효하지 않거나 캐스팅 실패 시 처리
        if (!bValidIcon || !Cast<UTexture2D>(Template->IconTexture))
        {
            ItemIcon->SetBrush(FSlateBrush()); // 빈 브러시 설정
            ItemIcon->SetVisibility(ESlateVisibility::Collapsed);

            UE_LOG(LogTemp, Warning, TEXT("[UI] IconTexture is NULL or invalid for item: %s"),
                Template ? *Template->GetName() : TEXT("NULL"));
        }
    }



    // 수량 텍스트
    if (QuantityText)
    {
        if (bHasItem)
        {
            const int32 MaxStack = (Template && Template->MaxStackCount > 0) ? Template->MaxStackCount : 1;

            if (CachedQuantity > 1 || MaxStack > 1)
            {
                QuantityText->SetText(FText::AsNumber(CachedQuantity));
                QuantityText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
            }
            else
            {
                QuantityText->SetText(FText::GetEmpty());
                QuantityText->SetVisibility(ESlateVisibility::Collapsed);
            }

            UE_LOG(LogTemp, Log, TEXT("[UI] Quantity Text Set: %d / MaxStack: %d"), CachedQuantity, MaxStack);
        }
        else
        {
            QuantityText->SetText(FText::GetEmpty());
            QuantityText->SetVisibility(ESlateVisibility::Collapsed);

            UE_LOG(LogTemp, Log, TEXT("[UI] Quantity Text Hidden (No item)"));
        }
    }
    // 블루프린트 확장용 이벤트
    K2_OnDisplayDataUpdated();
}
