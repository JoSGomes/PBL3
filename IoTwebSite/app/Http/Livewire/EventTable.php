<?php

namespace App\Http\Livewire;

use Livewire\Component;
use App\Models\Event;

class EventTable extends Component
{
    public $events;
    public $day;
    public $month;

    public function render()
    {
        return view('livewire.event-table');
    }

    public function search(){
        $this->events = Event::where('month',$this->month)->where('day',$this->day)->get();
    }
}
