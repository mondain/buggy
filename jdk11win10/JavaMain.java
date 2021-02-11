
/**
 * Test class for debugging issue encountered on Windows 10 with OpenJDK 11.
 */
public class JavaMain {

    public native long open();

    public native void close(long id);

    public static void main(String[] args) {
        System.loadLibrary("main");
        JavaMain m = new JavaMain();
        System.out.println("Opening main");
        long id = m.open();
        System.out.printf("Opened main for id: %s; now attempting close%n", Long.toUnsignedString(id));
        m.close(id);
        System.out.println("Closed main");
    }

}
