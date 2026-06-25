
#pragma once

#include "SDK/Engine_classes.hpp"
#include "SDK/UWECrafting_classes.hpp"

struct StepData {
    int x = 0, y = 0, width = 256, height = 256;
    int pivot_x = 128, pivot_y = 128;
    float rotation = 0.0f;
    SDK::FLinearColor color = SDK::FLinearColor(1, 1, 1, 1);
    SDK::EBlendMode blendMode = SDK::EBlendMode::BLEND_Translucent;
};

struct DrawingStep {
    StepData step = {};
    SDK::UTexture2D* texture = nullptr;
};

class IconBuilder {
    std::vector<DrawingStep> drawingSteps;

    static SDK::TArray<uint8_t> BuildFakeBitmap(UC::TArray<SDK::FColor> array);
    static SDK::UTexture2D *CreateTextureFromRenderTarget(SDK::UTextureRenderTarget2D* target);

public:
    IconBuilder();

    IconBuilder *addIcon(SDK::UTexture2D* tex, const StepData &data = StepData());

    IconBuilder *addIconFromItem(const SDK::UUWEItemType* item, const StepData &data = StepData());
    IconBuilder *addIconFromItem(const std::string &itemId, const StepData &data = StepData());

    IconBuilder *addIconFromRecipe(const SDK::UUWECraftingRecipe* recipe, const StepData &data = StepData());
    IconBuilder *addIconFromRecipe(const std::string &recipeId, const StepData &data = StepData());

    SDK::UTexture2D *build();
    SDK::TSoftObjectPtr<SDK::UTexture2D> buildWrapped();
};
