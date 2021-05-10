/**
 * Created by frkn on 04.12.2017.
 */

public enum Action {
    LIST((byte) 0x22),
    READ((byte) 0x55),
    REQUEST((byte) 0xAA),
    UPDATE((byte) 0xCC);

    private byte code;

    Action(byte code) {
        this.code = code;
    }

    public byte code() {
        return code;
    }
}
