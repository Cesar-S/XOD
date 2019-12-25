
// clang-format off
{{#global}}
// clang-format on

#include <Line.h>

// clang-format off
{{/global}}
// clang-format on

struct State {
    uint8_t mem[sizeof(Line)];
    Line* line;
    int16_t x0, y0, x1, y1;
};

// clang-format off
{{ GENERATED_CODE }}
// clang-format on

void evaluate(Context ctx) {
    auto state = getState(ctx);

    auto gfx = getValue<input_GFX>(ctx);

    int16_t x0 = (int16_t)getValue<input_X0>(ctx);
    int16_t y0 = (int16_t)getValue<input_Y0>(ctx);
    int16_t x1 = (int16_t)getValue<input_X1>(ctx);
    int16_t y1 = (int16_t)getValue<input_Y1>(ctx);

    if (isSettingUp()) {
        state->line = new (state->mem) Line(gfx, x0, y0, x1, y1);
        emitValue<output_GFXU0027>(ctx, state->line);
    }

    if (x0 != state->x0 || y0 != state->y0 || x1 != state->x1 || y1 != state->y1) {
        state->x0 = x0;
        state->y0 = y0;
        state->x1 = x1;
        state->y1 = y1;
        state->line->setParams(x0, y0, x1, y1);
        emitValue<output_GFXU0027>(ctx, state->line);
    }

    if (isInputDirty<input_GFX>(ctx)) {
        emitValue<output_GFXU0027>(ctx, state->line);
    }
}
