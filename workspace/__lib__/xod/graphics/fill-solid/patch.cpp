
// clang-format off
{{#global}}
// clang-format on

#include <FillSolid.h>

// clang-format off
{{/global}}
// clang-format on

struct State {
    uint8_t mem[sizeof(FillSolid)];
    FillSolid* fillSolid;
};

// clang-format off
{{ GENERATED_CODE }}
// clang-format on

void evaluate(Context ctx) {
    auto state = getState(ctx);

    auto gfx = getValue<input_GFX>(ctx);
    auto color = getValue<input_CLR>(ctx);

    if (isSettingUp()) {
        state->fillSolid = new (state->mem) FillSolid(gfx, color);
        emitValue<output_GFXU0027>(ctx, state->fillSolid);
    }

    if (isInputDirty<input_CLR>(ctx)) {
        state->fillSolid->setParams(color);
        emitValue<output_GFXU0027>(ctx, state->fillSolid);
    }

    if (isInputDirty<input_GFX>(ctx)) {
        emitValue<output_GFXU0027>(ctx, state->fillSolid);
    }
}
