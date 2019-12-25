
// clang-format off
{{#global}}
// clang-format on

#include <Stroke.h>

// clang-format off
{{/global}}
// clang-format on

struct State {
    uint8_t mem[sizeof(Stroke)];
    Stroke* stroke;
};

// clang-format off
{{ GENERATED_CODE }}
// clang-format on

void evaluate(Context ctx) {
    auto state = getState(ctx);

    auto gfx = getValue<input_GFX>(ctx);
    auto color = getValue<input_CLR>(ctx);

    if (isSettingUp()) {
        state->stroke = new (state->mem) Stroke(gfx, color);
        emitValue<output_GFXU0027>(ctx, state->stroke);
    }

    if (isInputDirty<input_CLR>(ctx)) {
        state->stroke->setParams(color);
        emitValue<output_GFXU0027>(ctx, state->stroke);
    }

    if (isInputDirty<input_GFX>(ctx)) {
        emitValue<output_GFXU0027>(ctx, state->stroke);
    }
}
