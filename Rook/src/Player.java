import java.util.Scanner;

public class Player {
    public String name;
    public Card[] possibleCards = Card.ALL_CARDS;
    public int bid;
    private Scanner input = new Scanner(System.in);

    public int getBid() {
        System.out.println("Enter " + name + "'s bid, or pass");
        String inputString = input.nextLine().toLowerCase();
        if (inputString.equals("pass")) {
            bid = -1;
        } else {
            bid = Integer.parseInt(inputString);
        }
        return bid;
    }

    public Player(String name) {
        this.name = name;
    }
}
