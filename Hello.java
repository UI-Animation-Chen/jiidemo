public class Hello {

	public static void main(String[] args) {
		System.out.println("java main start");
		//System.out.println(args[0]);
		new Hello().nativeThreadId();
        showThread();
		System.out.println("java main exit");
	}
    
    public static void showThread() {
        Thread t = Thread.currentThread();
        System.out.println("name:" + t.getName() + ", id:" + t.getId());
    }
    
    // ----- native method -----

	public native void nativeThreadId();

}
