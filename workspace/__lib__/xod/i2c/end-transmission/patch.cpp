#pragma XOD error_raise enable

struct State {
};

{{ GENERATED_CODE }}

void evaluate(Context ctx) {
    if (!isInputDirty<input_SEND>(ctx))
        return;

    auto wire = getValue<input_I2C>(ctx);

    if (wire->endTransmission() == 0) {
        emitValue<output_DONE>(ctx, 1);
    } else {
        raiseError(ctx);
    }
}
