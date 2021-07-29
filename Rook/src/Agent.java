import java.util.ArrayList;
import java.util.Scanner;

public class Agent {
    public Player[] players = new Player[4];
    static Scanner input = new Scanner(System.in);
    public Player me;
    public ArrayList<Card> cards = new ArrayList<Card>();

    // public static
    public void play() {
        // populate players array
        Player me = new Player("me");
        players[0] = me;
        for (int i = 1; i < 4; i++) {
            System.out.println(
                    "Enter name of player " + i + ", starting from my left");
            players[i] = new Player(input.nextLine());
        }
        // get hand
        int i = 1;
        while (cards.size() < 5) {
            System.out.println(
                    "Enter card #" + i + " in format 'color rank', or 'rook'");
            String currentCard = input.nextLine();
            if (currentCard.equals("rook")) {
                cards.add(new Card("rook", "rook"));
            } else {
                int spaceIndex = currentCard.indexOf(' ');
                String suit = currentCard.substring(0, spaceIndex);
                String rank = currentCard.substring(spaceIndex + 1);
                cards.add(new Card(suit, rank));
            }
            i++;
        }
        for (Card card : cards) {
            System.out.println(card.toString());
        }
    }
}
