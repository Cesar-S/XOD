
struct State {
};

// clang-format off
{{ GENERATED_CODE }}
// clang-format on

void evaluate(Context ctx) {
    auto bitmap = getValue<input_BMP>(ctx);
    auto dev = getValue<input_DEV>(ctx);

    if (isInputDirty<input_BMP>(ctx)) {
        bitmap->renderScreen(dev);
    }
    emitValue<output_DEVU0027>(ctx, dev);
}
