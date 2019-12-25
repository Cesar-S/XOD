
// clang-format off
{{#global}}
// clang-format on

#include <Canvas.h>

// clang-format off
{{/global}}
// clang-format on

struct State {
    uint8_t mem[sizeof(Canvas)];
    Canvas* canvas;
};

// clang-format off
{{ GENERATED_CODE }}
// clang-format on

void evaluate(Context ctx) {

    auto state = getState(ctx);
    auto color = getValue<input_BG>(ctx);

    if (isSettingUp()) {
        state->canvas = new (state->mem) Canvas(color);
        emitValue<output_GFX>(ctx, state->canvas);
    }

    if (isInputDirty<input_BG>(ctx)) {
        state->canvas->setParams(color);
        emitValue<output_GFX>(ctx, state->canvas);
    }
}
