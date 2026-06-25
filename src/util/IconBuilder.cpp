//
// Created by Limo on 28/05/2026.
//

#include "util/IconBuilder.hpp"

//#include "util/BufferBuilder.hpp"
#include "util/Finders.hpp"
#include "FWorldContext.hpp"
#include "util/Log.hpp"

using namespace SDK;

constexpr int IconSize = 256;
constexpr int DataSize = 4 * IconSize * IconSize;

TArray<uint8_t> IconBuilder::BuildFakeBitmap(TArray<FColor> array) {
    BufferBuilder builder(DataSize + 54);

    // File Header
    builder.add<uint16_t>(0x4D42);
    builder.add<uint32_t>(DataSize + 54);
    builder.add<uint16_t>(0);
    builder.add<uint16_t>(0);
    builder.add<uint32_t>(54);

    // Bitmap Header
    builder.add<uint32_t>(40);
    builder.add<int32_t>(IconSize);
    builder.add<int32_t>(-IconSize);
    builder.add<uint16_t>(1);
    builder.add<uint16_t>(32);
    builder.add<uint32_t>(0);
    builder.add<uint32_t>(DataSize);
    builder.add<int32_t>(0);
    builder.add<int32_t>(0);
    builder.add<uint32_t>(0);
    builder.add<uint32_t>(0);

    // Image Data
    for (const auto [b, g, r, a] : array) {
        builder.add<uint8_t>(b);
        builder.add<uint8_t>(g);
        builder.add<uint8_t>(r);
        builder.add<uint8_t>(a);
    }

    return builder.buildTArray();
}

UTexture2D *IconBuilder::CreateTextureFromRenderTarget(UTextureRenderTarget2D *target) {
    if (!target)
        return nullptr;

    TArray<FColor> colors;
    UKismetRenderingLibrary::ReadRenderTarget(UWorld::GetWorld(), target, &colors, false);
    Log::Warning("6");

    const auto bitmap = BuildFakeBitmap(colors);
    Log::Warning("7");
    return UKismetRenderingLibrary::ImportBufferAsTexture2D(UWorld::GetWorld(), bitmap);
}

IconBuilder::IconBuilder() = default;

IconBuilder *IconBuilder::addIcon(UTexture2D *tex, const StepData &data) {
    drawingSteps.emplace_back(DrawingStep(data, tex));
    return this;
}

IconBuilder *IconBuilder::addIconFromItem(const UUWEItemType *item, const StepData &data) {
    if (!item)
        return this;
    return addIcon(item->Thumbnail.Get(), data);
}

IconBuilder *IconBuilder::addIconFromItem(const std::string &itemId, const StepData &data) {
    return addIconFromItem(Finders::searchItem(itemId), data);
}

IconBuilder *IconBuilder::addIconFromRecipe(const UUWECraftingRecipe *recipe, const StepData &data) {
    if (!recipe)
        return this;
    return addIcon(recipe->Thumbnail.Get(), data);
}

IconBuilder * IconBuilder::addIconFromRecipe(const std::string &recipeId, const StepData &data) {
    return addIconFromRecipe(Finders::searchRecipe(recipeId), data);
}

UTexture2D *IconBuilder::build() {
    const auto worldCtx = UWorld::GetWorld();

    Log::Warning("1");
    const auto renderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(
        worldCtx, IconSize, IconSize, ETextureRenderTargetFormat::RTF_RGBA8,
        FLinearColor{0, 0, 0, 0}, false, false
    );
    Log::Warning("2 {:p}", (void*) renderTarget);

    Log::Warning("RT: {:p}", (void*)renderTarget);
    Log::Warning("SizeX: {}", renderTarget->SizeX);
    Log::Warning("SizeY: {}", renderTarget->SizeY);
    Log::Warning("Format: {}", (int)renderTarget->RenderTargetFormat);

    UCanvas* canvas;
    FVector2D size(IconSize, IconSize);
    FDrawToRenderTargetContext ctx{};

    UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(worldCtx, renderTarget, &canvas, &size, &ctx);
    Log::Warning("3");Log::Warning("RT valid: {}", renderTarget != nullptr);
    Log::Warning("RT size: {}x{}", renderTarget->SizeX, renderTarget->SizeY);
    Log::Warning("World: {:p}", (void*)worldCtx);

    for (auto [data, texture] : drawingSteps) {
        const auto [x, y, width, height, pivotX, pivotY, rotation, color, blendMode] = data;
        Log::Warning("Hm {:p}", (void*) canvas);
        canvas->K2_DrawLine(FVector2D(0, 0), FVector2D(1, 1), 5, color);
        /*canvas->K2_DrawTexture(
            texture,
            FVector2D(x, y),
            FVector2D(width, height),
            FVector2D(0, 0),
            FVector2D(1, 1),
            color,
            blendMode,
            rotation,
            FVector2D(pivotX, pivotY)
        );*/
    }
    Log::Warning("4");
    UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(worldCtx, ctx);
    Log::Warning("5");

    const auto tex = CreateTextureFromRenderTarget(renderTarget);
    Log::Warning("8");
    UKismetRenderingLibrary::ReleaseRenderTarget2D(renderTarget);
    Log::Warning("9");
    return tex;
}

TSoftObjectPtr<UTexture2D> IconBuilder::buildWrapped() {
    return static_cast<TSoftObjectPtr<UTexture2D>>(UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(build()));
}
