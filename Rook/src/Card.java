import java.util.Map;
import java.util.Set;

public class Card {
    public static final Map<String, Integer> pointMap = Map.of("1", 15, "14",
            10, "10", 10, "5", 5, "rook", 20);
    public static final Map<String, Integer> valueExceptions = Map.of("1", 15,
            "rook", 1);
    public static final Set<String> suits = Set.of("red", "yellow", "black",
            "green");
    public static final Set<String> ranks = Set.of("2", "3", "4", "5", "6", "7",
            "8", "9", "10", "11", "12", "13", "14", "1");
    public static Card[] ALL_CARDS;
    static {
        ALL_CARDS = new Card[suits.size() * ranks.size() + 1];
        int index = 0;
        for (String suit : suits) {
            for (String rank : ranks) {
                ALL_CARDS[index] = new Card(suit, rank);

            }
        }
    }
    public String suit;
    public String rank;
    public int points;
    public int value;

    public String toString() {
        return rank.equals("rook") ? "rook" : this.suit + " " + this.rank;
    }

    public Card(String suit, String rank) {
        this.suit = suit;
        this.rank = rank;
        this.points = pointMap.containsKey(rank) ? pointMap.get(rank) : 0;
        this.value = valueExceptions.containsKey(rank)
                ? valueExceptions.get(rank)
                : Integer.parseInt(rank, 10);

    }
}
