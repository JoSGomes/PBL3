@extends('layouts.template')

@section('body')
    <div class="container">
        <div class="row">
            <div class="col">
                <livewire:state-device/>
                <livewire:btn-alarm/>
            </div>

            <div class="col">
                <h1 class="subtitle">Contatos de emergência</h1>
                <div class="scrollable position-relative top-50">
                    <table class="table table-hover table-bordeless">
                        <thead>
                        <tr>
                            <th scope="col">Nome</th>
                            <th scope="col">Número</th>
                        </tr>
                        </thead>
                        <tbody>
                        @foreach ($contacts as $contact)
                            <tr >
                                 <td>{{$contact->name}}</td>
                                 <td>{{$contact->number}}</td>
                            </tr>
                        @endforeach
                        </tbody>
                    </table>
                </div>

            </div>
        </div>
    </div>
@endsection


