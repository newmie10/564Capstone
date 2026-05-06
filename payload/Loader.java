public class Loader {
    public static void main(String[] args) {
        System.out.println("Starting loader application...");
        try {
            // This triggers the static block of SafePayload
            Class.forName("Beachhead");
        } catch (ClassNotFoundException e) {
            System.out.println("Could not find the class.");
        }
        System.out.println("Loader application finished.");
    }
}
