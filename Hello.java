public class Hello {

	public static void main(String[] args) {
		//System.out.println(args[0]);
		//new Hello().javaCall();
        showThread();
	}
    
    public static void showThread() {
        System.out.println("----- native invoke -----");
        Thread t = Thread.currentThread();
        System.out.println("name:" + t.getName() + ", id:" + t.getId());
    }
    
    // ----- native method -----

	public native void javaCall();

}
