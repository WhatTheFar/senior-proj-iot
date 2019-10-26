new Vue({
  el: "#vue-app",
  data: {
    numberOfPeople: 0,
    currentNumberOfPeople: 10
  },
  methods: {
    setNumberOfPeople: function() {
      console.log(this.numberOfPeople);
      //   this.$http.post("action.php", this.numberOfPeople, function(data) {
      //
      //   });
    },
    resetNumberOfPeople: function() {
      //   this.$http.post("action.php", this.numberOfPeople, function(data) {
      //
      //   });
    }
  }
});
